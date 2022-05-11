#include <unordered_map>

#include "SDL_events.h"
#include "SDL_mutex.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SUI_in_events.h"
#include "SUI_in_window_data.h"
#include "SUI_in_window_manager.h"
#include "SUI_window.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"

namespace sui {

/**
* use this function is to solve that when some object style change the window can't redraw auto, the window was waitting the EXPOSED event
* @todo don't use this to decrease the occupy of CPU
* @todo this function should be static, but friend function can't set to static, avoid friend function!
* this function is not used now
*/
/*static */unsigned interval_deal(unsigned interval, void *data) {
    Window_manager *pwm = static_cast<Window_manager*>(data);
    pwm->update_all_window();
    return interval;
}

Window_manager::Window_manager() {}

Window_manager::~Window_manager() {
    DBG(<< "Window manager was deleted");
}

Window_manager *Window_manager::get_window_manager() {
    static Window_manager wm;
    return &wm;
}

void Window_manager::add_window(Window *pWindow, bool flag) {
    window_map[pWindow->get_window_id()] = std::make_pair(pWindow, flag);
}

void Window_manager::remove_window(Uint32 window_no) {
    DBG(<< "window manager remove " << window_map[window_no].first->get_name() << "(window id:" << window_no << ")");
    window_map.erase(window_no);
    // in multiwindows program, close window will not send the SDL_QUIT event
    // if the program can quit?
    could_quit();
}

void Window_manager::remove_window(Window *pWindow) {
    remove_window(pWindow->get_window_id());
}

void Window_manager::patch_event_to(Uint32 window_no, const SDL_Event &event, Window_status statu) {
    bool flag = true;
    // SDL_LockMutex(lock_window_map);
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
    // SDL_UnlockMutex(lock_window_map);
}

void Window_manager::patch_event_to_all(const SDL_Event &event, Window_status statu) {
    for (auto iter = window_map.begin(); iter != window_map.end(); ++iter) {
        bool wnd_statu = iter->second.second;
        if (statu == window_status_all
        || (statu == window_status_showing && wnd_statu)
        || (statu == window_status_hiding && !wnd_statu)) {
            push_event(iter->second.first, event);
        }
    }
}

void Window_manager::update_all_window() {
    for (const auto &p : window_map) {
        if (p.second.second) {
            update_window(p.second.first);
        }
    }
}

void Window_manager::update_window(Window *pWindow) {
    pWindow->redraw_all();
}

Window *Window_manager::get_window(Uint32 window_no) {
    Window *pWnd = nullptr;
    if (window_map.find(window_no) != window_map.end()) {
        pWnd = window_map[window_no].first;
    }
    return pWnd;
}

// if all window are closed
void Window_manager::could_quit() {
    if (window_map.empty()) {
        static SDL_Event event;
        event.type = Event_type::quit_after_all_window_closed;
        // event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }
}

void Window_manager::push_event(Window *pWnd, const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        break;
    case SDL_KEYUP:
        break;
    case SDL_MOUSEBUTTONDOWN:
        break;
    case SDL_MOUSEBUTTONUP: {
        Mouse_button_event mbe{event.button};
        pWnd->deal_mouse_button_down_event(mbe);
        break;
    }
    case SDL_MOUSEWHEEL:
        break;
    case SDL_MOUSEMOTION:
        break;
    case SDL_WINDOWEVENT: {
        Event w_e{event};
        switch (event.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            pWnd->deal_window_close_event(w_e);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            update_window(pWnd);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pWnd->deal_window_resized_event(w_e);
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

}