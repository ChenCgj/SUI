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
* this function is not used now
*/
// static unsigned interval_deal(unsigned interval, void *data) {
//     Window_manager *pwm = static_cast<Window_manager*>(data);
//     pwm->update_all_window();
//     return interval;
// }

Window_manager::Window_manager() {}

Window_manager::~Window_manager() {
    DBG(<< "Window manager was deleted");
}

Window_manager *Window_manager::get_window_manager() {
    static Window_manager wm;
    return &wm;
}

void Window_manager::add_window(Window *pWindow, Window_listen_status statu) {
    window_map[pWindow->get_window_id()] = std::make_pair(pWindow, statu);
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

void Window_manager::patch_event_to(Uint32 window_no, const SDL_Event &event, Window_listen_status statu) {
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
        Window_listen_status wnd_lis_statu = window_map[window_no].second;
        if (statu == window_listening_ignore || (statu == wnd_lis_statu)) {
            push_event(pWnd, event);
        }
    }
    // SDL_UnlockMutex(lock_window_map);
}

void Window_manager::patch_event_to_all(const SDL_Event &event, Window_listen_status statu) {
    for (auto iter = window_map.begin(); iter != window_map.end(); ++iter) {
        Window_listen_status wnd_lis_statu = iter->second.second;
        if (statu == window_listening_ignore || (statu == wnd_lis_statu)) {
            push_event(iter->second.first, event);
        }
    }
}

void Window_manager::update_all_window() {
    for (const auto &p : window_map) {
        if (p.second.second == window_message_listening) {
            update_window(p.second.first);
        }
    }
}

void Window_manager::update_window(Window *pWindow) {
    pWindow->redraw();
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
    case SDL_KEYDOWN: {
        Keyboard_event key_down{event};
        pWnd->deal_key_down_event(key_down);
        break;
    }
    case SDL_KEYUP: {
        Keyboard_event key_up{event};
        pWnd->deal_key_up_event(key_up);
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        Mouse_button_event mouse_button_down{event};
        pWnd->deal_mouse_button_down_event(mouse_button_down);
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        Mouse_button_event mouse_button_up{event};
        pWnd->deal_mouse_button_up_event(mouse_button_up);
        break;
    }
    case SDL_MOUSEWHEEL: {
        Mouse_wheel_event mouse_wheel{event};
        pWnd->deal_mouse_wheel_event(mouse_wheel);
        break;
    }
    case SDL_MOUSEMOTION: {
        Mouse_motion_event mouse_move{event};
        pWnd->deal_mouse_move_event(mouse_move);
        break;
    }
    case SDL_WINDOWEVENT: {
        Event window_event{event};
        switch (event.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            pWnd->deal_window_close_event(window_event);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            update_window(pWnd);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pWnd->deal_window_resized_event(window_event);
            break;
        default:
            break;
        }
        break;
    }
    default: {
        Event e_other{event};
        pWnd->deal_other_event(e_other);
        break;
    }
    }
}
}