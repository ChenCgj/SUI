#include "SDL.h"
#include "SDL_filesystem.h"
#include "SDL_ttf.h"

#include "SDL_version.h"
#include "SUI_object.h"
#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_window.h"
#include "SUI_in_debug.h"
#include "SUI_in_events.h"
#include <fstream>

using namespace sui;

static void clean();
static int event_filter(void *data, SDL_Event *event);

int main(int argc, char *argv[]) {
    SDL_version version;
    SDL_VERSION(&version);
    DBG(<< "SDL version: major: " << int(version.major) << " minor: " << int(version.minor) << " patch: " << int(version.patch));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    SDL_SetEventFilter(event_filter, nullptr);
    // call the user function
    SUI_main(argc, argv);
    SDL_Event event;
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
                WINDOW_MANAGER->patch_event_to_all(event, sui::Window_manager::window_listening_ignore);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                WINDOW_MANAGER->patch_event_to(event.key.windowID, event, sui::Window_manager::window_message_listening);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                WINDOW_MANAGER->patch_event_to(event.button.windowID, event, sui::Window_manager:: window_message_listening);
                break;
            case SDL_MOUSEWHEEL:
                WINDOW_MANAGER->patch_event_to(event.wheel.windowID, event, sui::Window_manager::  window_message_listening);
                break;
            case SDL_MOUSEMOTION:
                WINDOW_MANAGER->patch_event_to(event.motion.windowID, event, sui::Window_manager:: window_message_listening);
                break;
            case SDL_WINDOWEVENT:
                WINDOW_MANAGER->patch_event_to(event.window.windowID, event, sui::Window_manager:: window_message_listening);
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

int event_filter(void *data, SDL_Event *event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        WINDOW_MANAGER->patch_event_to(event->window.windowID, *event, sui::Window_manager::window_message_listening);
        return 0;
    }
    if (event->type == SDL_QUIT) {
        return 1;
    }
    return 1;
}

void clean() {
    TTF_Quit();
    SDL_Quit();
    DBG(<< "SDL_Quit OK");
}