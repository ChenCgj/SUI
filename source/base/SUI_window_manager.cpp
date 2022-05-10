#include <unordered_map>

#include "SDL_events.h"
#include "SDL_mutex.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SUI_in_events.h"
#include "SUI_in_window_data.h"
#include "SUI_in_window_manager.h"
#include "SUI_window.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"

namespace sui {

// which was created in SUI_window.cpp, element in it will be destroyed by window
extern std::unordered_map<Window *, std::pair<SDL_mutex *, SDL_cond *>> window_message_queue_lock_map;
// which was create in SUI_window.cpp, will be destroyed by the last window
extern SDL_mutex *lock_window_message_queue_lock_map;
// which is used to lock the window_id --- (Window *, flag) map, deleted by ~Window_manager()
static SDL_mutex *lock_window_map = nullptr;

/**
* use this function is to solve that when some object style change the window can't redraw auto, the window was waitting the EXPOSED event
* @todo don't use this to decrease the occupy of CPU
* @todo this function should be static, but friend function can't set to static, avoid friend function!
* this function is not used now
*/
/*static */unsigned interval_deal(unsigned interval, void *data) {
    Window_manager *pwm = static_cast<Window_manager*>(data);
    // static SDL_Event event;
    // event.type = Event_type::window_redraw_event;
    // event.user.type = event.type;
    // SDL_LockMutex(lock_window_map);
    // // all window will be redrawed
    // for (const auto &p : pwm->window_map) {
    //     event.user.windowID = p.first;
    //     event.user.data1 = p.second.first;
    //     if (p.second.second) {
    //         SDL_PushEvent(&event);
    //     }
    // }
    // SDL_UnlockMutex(lock_window_map);
    pwm->update_all_window();
    return interval;
}

Window_manager::Window_manager() {
    lock_window_map = SDL_CreateMutex();
    // SDL_AddTimer(1000 / 60, interval_deal, static_cast<void*>(this));
}

Window_manager::~Window_manager() {
    SDL_DestroyMutex(lock_window_map);
    lock_window_map = nullptr;
    DBG(<< "Window manager was deleted");
}

Window_manager *Window_manager::get_window_manager() {
    // use static variable not new, you will not when to delete it is correctly
    static Window_manager wm;
    return &wm;
}

void Window_manager::add_window(Window *pWindow, bool flag) {
    SDL_LockMutex(lock_window_map);
    window_map[pWindow->get_window_id()] = std::make_pair(pWindow, flag);
    SDL_UnlockMutex(lock_window_map);
}

void Window_manager::remove_window(Uint32 window_no) {
    SDL_LockMutex(lock_window_map);
    DBG(<< "window manager remove " << window_map[window_no].first->get_name() << "(window id:" << window_no << ")");
    window_map.erase(window_no);
    SDL_UnlockMutex(lock_window_map);
    // in multiwindows program, close window will not send the SDL_QUIT event
    // if the program can quit?
    could_quit();
}

void Window_manager::remove_window(Window *pWindow) {
    remove_window(pWindow->get_window_id());
}

void Window_manager::patch_event_to(Uint32 window_no, const SDL_Event &event, Window_status statu) {
    bool flag = true;
    SDL_LockMutex(lock_window_map);
    /**
    * @bug window ID 0??
    * @bug window has been closed (will be hiden for a while) and lost focus will get the event but the managet has delete the window when the window close
    */
    if (window_map.count(window_no) == 0) {
        DBG(<< "window manager recive a event can't deal. " << " window id: " << window_no << ". window_event: " << int(event.window.event));
        flag = false;
    }
    if (flag) {
        Window *pWnd = window_map[window_no].first;
        bool wnd_statu = window_map[window_no].second;
        if (statu == window_status_all
        || (statu == window_status_showing && wnd_statu)
        || (statu == window_status_hiding && !wnd_statu)) {
            push_event(pWnd, event);
        }
    }
    SDL_UnlockMutex(lock_window_map);
}

void Window_manager::patch_event_to_all(const SDL_Event &event, Window_status statu) {
    SDL_LockMutex(lock_window_map);
    for (auto iter = window_map.begin(); iter != window_map.end(); ++iter) {
        bool wnd_statu = iter->second.second;
        if (statu == window_status_all
        || (statu == window_status_showing && wnd_statu)
        || (statu == window_status_hiding && !wnd_statu)) {
            push_event(iter->second.first, event);
        }
    }
    SDL_UnlockMutex(lock_window_map);
}

void Window_manager::update_all_window() {
    SDL_LockMutex(lock_window_map);
    for (const auto &p : window_map) {
        if (p.second.second) {
            update_window(p.second.first);
        }
    }
    SDL_UnlockMutex(lock_window_map);
}

void Window_manager::update_window(Window *pWindow) {
    static SDL_Event e;
    e.type = Event_type::window_redraw_event;
    e.user.type = e.type;
    e.user.data1 = pWindow;
    SDL_PushEvent(&e);
}

Window *Window_manager::get_window(Uint32 window_no) {
    Window *pWnd = nullptr;
    SDL_LockMutex(lock_window_map);
    if (window_map.find(window_no) != window_map.end()) {
        pWnd = window_map[window_no].first;
    }
    SDL_UnlockMutex(lock_window_map);
    return pWnd;
}

// if all window are closed
void Window_manager::could_quit() {
    SDL_LockMutex(lock_window_map);
    if (window_map.empty()) {
        static SDL_Event event;
        event.type = Event_type::quit_after_all_window_closed;
        // event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }
    SDL_UnlockMutex(lock_window_map);
}

void Window_manager::push_event(Window *pWnd, const SDL_Event &event) {
    // get the message queue lock
    SDL_LockMutex(lock_window_message_queue_lock_map);
    SDL_mutex *m = window_message_queue_lock_map[pWnd].first;
    SDL_cond *c = window_message_queue_lock_map[pWnd].second;
    SDL_UnlockMutex(lock_window_message_queue_lock_map);
    SDL_LockMutex(m);
    pWnd->pData->event_queue.push(event);
    SDL_CondSignal(c);
    SDL_UnlockMutex(m);
}
}