/**
* @file SUI_in_window_manager.h
* @brief contains the definition of Window_manager, this class is designed to manage window and message
*/

#ifndef SUI_IN_WINDOW_MANAGER_H
#define SUI_IN_WINDOW_MANAGER_H

#include <map>

#include "SDL_video.h"
#include "SDL_render.h"
#include "SDL_events.h"

namespace sui {

class Window;
class Window_manager {
public:
    enum Window_listen_status {
        window_message_unlistening, window_message_listening, window_listening_ignore
    };
    static Window_manager *instance();
    void add_window(Window *pWindow, Window_listen_status statu, SDL_Window *pSDL_window = nullptr, SDL_Renderer *pRenderer = nullptr);
    void remove_window(Window *pWindow);
    void remove_window(Uint32 window_no);
    // the window_no param is the same as event.window.windowID 
    void patch_event_to(Uint32 window_no, const SDL_Event &event, Window_listen_status statu);
    void patch_event_to(Window *pWnd, const SDL_Event &event, Window_listen_status statu);
    void patch_event_to_all(const SDL_Event &event, Window_listen_status statu);
    void update_all_window();
    void update_window(Window *pWindow);
    void update_window(uint32_t window_no);
    void update_window_statu(Window *pWindow, Window_listen_status statu);
    Window *get_window(Uint32 window_no);
    void check_and_delete_window_data(Uint32 window_no);
    void update_texture_count(SDL_Renderer *pRenderer, int count);
    ~Window_manager();
    SDL_Window *get_sdl_window(const Window *pWindow);
    SDL_Renderer *get_sdl_renderer(const Window *pWindow);
    SDL_Renderer *get_window_id();
    SDL_Texture *get_render_target(SDL_Renderer *pRenderer);
    int set_render_target(SDL_Renderer *pRenderer, SDL_Texture *texture);
private:
    struct Window_data;
    std::map<Uint32, std::pair<Window *, Window_data>> window_map;
    std::map<SDL_Renderer *, SDL_Texture *> target_map;
    void push_event(Window *pw, const SDL_Event &event);
    long long get_window_by_renderer(SDL_Renderer *pRenderer);
    void could_quit();
    Window_manager();
    unsigned window_count;
};
}

#endif