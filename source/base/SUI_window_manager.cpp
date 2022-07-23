#include "SUI_in_events.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_event_handler_helper.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"
#include "SUI_window.h"

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

struct Window_manager::Window_data {
    SDL_Window *pWnd;
    SDL_Renderer *pRenderer;
    Window_manager::Window_listen_status listen_statu;
    long long texture_count;
    bool destroyable;
};

Window_manager::Window_manager() : window_count{0} {}

Window_manager::~Window_manager() {
    DBG(<< "Window manager was deleted");
}

Window_manager *Window_manager::instance() {
    // static Window_manager wm;
    // return &wm;
    static Window_manager *pwm = new Window_manager();
    return pwm;
}

void Window_manager::check_and_delete_window_data(Uint32 window_no) {
    Window_data &data = window_map[window_no].second;
    if (data.destroyable && data.texture_count == 0) {
        SDL_DestroyRenderer(data.pRenderer);
        SDL_DestroyWindow(data.pWnd);
        data.pRenderer = nullptr;
        data.pWnd = nullptr;
        window_map.erase(window_no);
        DBG(<< "delete window(window id: " << window_no << ")'s data ok.");
    }
}

long long Window_manager::get_window_by_renderer(SDL_Renderer *pRenderer) {
    for (auto iter = window_map.begin(); iter != window_map.end(); ++iter) {
        if (iter->second.second.pRenderer == pRenderer) {
            return iter->first;
        }
    }
    ERR(<< "get the invalid renderer.");
    return -1;
}

SDL_Window *Window_manager::get_sdl_window(const Window *pWindow) {
    Uint32 id = pWindow->get_window_id();
    // if (id == -1) {
    //     ERR(<< "this window doesn't register.");
    //     return nullptr;
    // }
    return window_map[id].second.pWnd;
}

SDL_Renderer *Window_manager::get_sdl_renderer(const Window *pWindow) {
    Uint32 id = pWindow->get_window_id();
    // if (id == -1) {
    //     ERR(<< "this window doesn't register.");
    //     return nullptr;
    // }
    return window_map[id].second.pRenderer;
}

void Window_manager::update_texture_count(SDL_Renderer *pRenderer, int count) {
    long long id = get_window_by_renderer(pRenderer);
    // id will be the UINT_MAX
    if (id == -1) {
        return;
    }
    window_map[id].second.texture_count += count;
    if (count < 0) {
        check_and_delete_window_data(id);
    }
    DBG(<< "window(id: " << id << ")'s texture count: " << window_map[id].second.texture_count << ".");
}

void Window_manager::add_window(Window *pWindow, Window_listen_status statu, SDL_Window *pSDL_window, SDL_Renderer *pRenderer) {
    Uint32 id = pWindow->get_window_id();
    if (window_map.find(id) == window_map.end()) {
        window_map[pWindow->get_window_id()] = std::make_pair(pWindow, Window_data{pSDL_window, pRenderer, statu, 0, false});
        ++window_count;
        DBG(<< "window manager add " << pWindow->get_name() << "(window id: " << id << ")");
    } else {
        ERR(<< "add a window has added register.");
    }
}

void Window_manager::update_window_statu(Window *pWindow, Window_listen_status statu) {
    Uint32 id = pWindow->get_window_id();
    if (window_map.count(id) == 0) {
        ERR(<< "not add this window to window manager.");
        return;
    }
    window_map[id].second.listen_statu = statu;
}

void Window_manager::remove_window(Uint32 window_no) {
    DBG(<< "window manager remove " << window_map[window_no].first->get_name() << "(window id:" << window_no << ")");
    // window_map.erase(window_no);
    window_map[window_no].second.destroyable = true;
    window_map[window_no].second.listen_statu = Window_manager::window_message_unlistening;
    check_and_delete_window_data(window_no);
    // in multiwindows program, close window will not send the SDL_QUIT event
    // if the program can quit?
    --window_count;
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
        Window_listen_status wnd_lis_statu = window_map[window_no].second.listen_statu;
        if (statu == window_listening_ignore || (statu == wnd_lis_statu)) {
            push_event(pWnd, event);
        }
    }
    // SDL_UnlockMutex(lock_window_map);
}

void Window_manager::patch_event_to_all(const SDL_Event &event, Window_listen_status statu) {
    for (auto iter = window_map.begin(); iter != window_map.end(); ++iter) {
        Window_listen_status wnd_lis_statu = iter->second.second.listen_statu;
        if (statu == window_listening_ignore || (statu == wnd_lis_statu)) {
            push_event(iter->second.first, event);
        }
    }
}

void Window_manager::update_all_window() {
    for (const auto &p : window_map) {
        if (p.second.second.listen_statu == window_message_listening) {
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
    if (window_count == 0) {
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
        HANDLER_HELPER->deal_key_down_event(pWnd, key_down);
        break;
    }
    case SDL_KEYUP: {
        Keyboard_event key_up{event};
        HANDLER_HELPER->deal_key_up_event(pWnd, key_up);
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        Mouse_button_event mouse_button_down{event};
        HANDLER_HELPER->deal_mouse_button_down_event(pWnd, mouse_button_down);
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        Mouse_button_event mouse_button_up{event};
        HANDLER_HELPER->deal_mouse_button_up_event(pWnd, mouse_button_up);
        break;
    }
    case SDL_MOUSEWHEEL: {
        Mouse_wheel_event mouse_wheel{event};
        HANDLER_HELPER->deal_mouse_wheel_event(pWnd, mouse_wheel);
        break;
    }
    case SDL_MOUSEMOTION: {
        Mouse_motion_event mouse_move{event};
        HANDLER_HELPER->deal_mouse_move_event(pWnd, mouse_move);
        break;
    }
    case SDL_WINDOWEVENT: {
        Event window_event{event};
        switch (event.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            HANDLER_HELPER->deal_window_close_event(pWnd, window_event);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            update_window(pWnd);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            HANDLER_HELPER->deal_window_resized_event(pWnd, window_event);
            break;
        default:
            break;
        }
        break;
    }
    default: {
        Event e_other{event};
        HANDLER_HELPER->deal_other_event(pWnd, e_other);
        break;
    }
    }
}

}