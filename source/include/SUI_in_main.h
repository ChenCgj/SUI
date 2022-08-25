#ifndef SUI_IN_MAIN_H
#define SUI_IN_MAIN_H

#include <functional>

#include "SDL_events.h"
#include "SUI_run_mode.h"
// #include "SUI_in_event_handler_helper.h"

#define MANAGERS Managers::instance()
#define ROOT MANAGERS->get_root()
#define TRASH_ROOT MANAGERS->get_trash_root()
#define WINDOW_MANAGER MANAGERS->get_window_manager()
#define HANDLER_HELPER Event_handler_helper::instance()
#define THEME_MANAGER MANAGERS->get_theme_manager()
#define TEXTURE_SDL_MANAGER MANAGERS->get_texture_sdl_manager()
#define TIMER_MANAGER MANAGERS->get_timer_manager()
#define INIT Managers::init

extern int SUI_main(int argc, char *argv[]);

namespace sui {

extern std::function<void (void)> clean_last;
extern Run_mode run_mode;

enum Event_type {
    none_event = SDL_USEREVENT,
    quit_after_all_window_closed,
    update_all_window,
    update_window
};

enum Mask_mode {
    be_masked,
    none_mask,
    to_mask
};
}
#endif