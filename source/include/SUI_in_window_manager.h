/**
* @file SUI_in_window_manager.h
* @brief contains the definition of Window_manager, this class is designed to manage window and message
*/

#ifndef SUI_IN_WINDOW_MANAGER_H
#define SUI_IN_WINDOW_MANAGER_H

#include <map>
#include <stdint.h>
#include <utility>

#include "SDL_events.h"
#include "SDL_video.h"

#include "SUI_window.h"

namespace sui {

class Window_manager {
public:
    enum Window_listen_status {
        window_message_unlistening, window_message_listening, window_listening_ignore
    };
    static Window_manager *get_window_manager();
    void add_window(Window *pWindow, Window_listen_status statu);
    void remove_window(Window *pWindow);
    void remove_window(Uint32 window_no);
    // he window_no param is the same as event.window.windowID 
    void patch_event_to(Uint32 window_no, const SDL_Event &event, Window_listen_status statu);
    void patch_event_to(Window *pWnd, const SDL_Event &event, Window_listen_status statu);
    void patch_event_to_all(const SDL_Event &event, Window_listen_status statu);
    void update_all_window();
    void update_window(Window *pWindow);
    void update_window(uint32_t window_no);
    Window *get_window(Uint32 window_no);
    ~Window_manager();
private:
    // <window_no, <Window *, is_showing flag>>
    std::map<Uint32, std::pair<Window *, Window_listen_status>> window_map;
    void push_event(Window *pw, const SDL_Event &event);
    void could_quit();
    Window_manager();
};
}

#endif