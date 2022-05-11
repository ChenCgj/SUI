#ifndef SUI_IN_MAIN_H
#define SUI_IN_MAIN_H

#include "SDL_events.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"
#define ROOT Object::get_root()
#define TRASH_ROOT Object::get_trash_root()
#define WINDOW_MANAGER Window_manager::get_window_manager()
extern int SUI_main(int argc, char *argv[]);

namespace sui {
// create in SUI_object.cpp and delete in SDL_main.cpp:clean()
extern SDL_mutex *lock_object_tree;
// create in SUI_window.cpp and deleted in SDL_main.cpp:clean()
extern SDL_mutex *lock_window_message_queue_lock_map;

enum Event_type {
    none_event = SDL_USEREVENT,
    quit_after_all_window_closed
};
}
#endif