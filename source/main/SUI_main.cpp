#include "SDL.h"

#include "SUI_object.h"
#include "SUI_in_main.h"
#include "SUI_in_window_manager.h"
#include "SUI_window.h"
#include "SUI_in_debug.h"
#include "SUI_in_events.h"

using namespace sui;

static void clean();
static int tackle_size_change(void *data, SDL_Event * resized_event);

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    // call the user function
    SUI_main(argc, argv);
    SDL_Event event;
    SDL_SetEventFilter(tackle_size_change, nullptr);
    bool flag = true;
    while (flag) {
        while (flag && SDL_WaitEventTimeout(&event, 10)) {
            switch (event.type) {
            case Event_type::quit_after_all_window_closed:
                flag = false;
                break;
            case SDL_QUIT:
                // maybe have some window not be closed, when we sent message to close them
                // we may exit the loop and clean the lock which is nedded when remove the window from the object_tree
                // flag = false;
                event.type = SDL_WINDOWEVENT;
                event.window.event = SDL_WINDOWEVENT_CLOSE;
                WINDOW_MANAGER->patch_event_to_all(event, sui::Window_manager::window_status_all);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                WINDOW_MANAGER->patch_event_to(event.key.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                WINDOW_MANAGER->patch_event_to(event.button.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_MOUSEWHEEL:
                WINDOW_MANAGER->patch_event_to(event.wheel.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_MOUSEMOTION:
                WINDOW_MANAGER->patch_event_to(event.motion.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_WINDOWEVENT:
                WINDOW_MANAGER->patch_event_to(event.window.windowID, event, sui::Window_manager::window_status_showing);
                break;
            default:
                break;
            }
        }
    }
    DBG(<< "Main circle exit");
    clean();
    DBG(<< "Main end");
    return 0;
}

int tackle_size_change(void *data, SDL_Event *resized_event) {
    if (resized_event->type == SDL_WINDOWEVENT && resized_event->window.event == SDL_WINDOWEVENT_RESIZED) {
        WINDOW_MANAGER->patch_event_to(resized_event->window.windowID, *resized_event, sui::Window_manager::window_status_showing);
        return 0;
    }
    return 1;
}

void clean() {
    SDL_Quit();
    DBG(<< "SDL_Quit OK");
}