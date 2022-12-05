#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_in_window_manager.h"
#include "SUI_tool.h"

namespace sui {

void present_all() {
    // WINDOW_MANAGER->update_all_window();
    if (run_mode == Run_mode::poll) {
        // in poll mode, we will update the window quickly
        return;
    }
    static SDL_Event event;
    event.type = Event_type::update_all_window;
    SDL_PushEvent(&event);
}

void quit() {
    static SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void register_clean(std::function<void (void)> func) {
    clean_last = func;
}

void present(Window *pWindow) {
    if (run_mode == Run_mode::poll) {
        // in poll mode, we will update the window quickly
        return;
    }
    static SDL_Event event;
    event.type = Event_type::update_window;
    event.user.data1 = static_cast<void *>(pWindow);
    SDL_PushEvent(&event);
}

void set_run_mode(Run_mode mode) {
    run_mode = mode;
}
}