#ifndef SUI_IN_MAIN_H
#define SUI_IN_MAIN_H

#include "SDL_events.h"
#include "SUI_in_event_handler_helper.h"

#define ROOT Object::get_root()
#define TRASH_ROOT Object::get_trash_root()
#define WINDOW_MANAGER Window_manager::get_window_manager()
#define HANDLER_HELPER Event_handler_helper::instance()
#define THEME_MANAGER Theme_manager::instance()

extern int SUI_main(int argc, char *argv[]);

namespace sui {

enum Event_type {
    none_event = SDL_USEREVENT,
    quit_after_all_window_closed
};
}
#endif