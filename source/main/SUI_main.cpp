// #include "SDL_hints.h"
#include "SDL_version.h"
#include "SDL_main.h"

#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_debug.h"
#include "SUI_tool.h"

namespace sui {
std::function<void (void)> clean_last = nullptr;
Run_mode run_mode = poll;
extern int clean_trash(void *data);
}

using namespace sui;

static void clean();
static int event_filter(void *data, SDL_Event *event);
static int get_event(SDL_Event *event, uint32_t interval);

int main(int argc, char *argv[]) {
    SDL_version version;
    SDL_VERSION(&version);
    DBG(<< "SDL version: major: " << int(version.major) << " minor: " << int(version.minor) << " patch: " << int(version.patch));

    INIT();
    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_SetEventFilter(event_filter, nullptr);
    // call the user function
    SUI_main(argc, argv);
    SDL_Event event;
    bool flag = true;
    while (flag) {
        // while (flag && SDL_WaitEventTimeout(&event, 10)) {
        while (flag && get_event(&event, 10)) {
            switch (event.type) {
            case Event_type::quit_after_all_window_closed:
                flag = false;
                break;
            case Event_type::update_all_window:
                WINDOW_MANAGER->update_all_window();
                break;
            case Event_type::update_window:
                WINDOW_MANAGER->update_window(static_cast<Window_base *>(event.user.data1));
                break;
            case SDL_QUIT:
                // maybe have some window not be closed, when we sent message to close them
                // we may exit the loop and clean the lock which is nedded when remove the window from the object_tree
                // flag = false;
                event.type = SDL_WINDOWEVENT;
                event.window.event = SDL_WINDOWEVENT_CLOSE;
                WINDOW_MANAGER->patch_event_to_all(event, sui::Window_manager::window_listening_ignore);
                break;
            case SDL_WINDOWEVENT:
                WINDOW_MANAGER->patch_event_to(event.window.windowID, event, sui::Window_manager:: window_message_listening);
                break;
            default:
                break;
            }
        }
        clean_trash(nullptr);
        // present_all();
        if (flag) {
            // if we want to quit, we should not update it.
            WINDOW_MANAGER->update_all_window();
        }
    }
    DBG(<< "Main circle exit");
    clean();
    DBG(<< "Main end");
    return 0;
}

int event_filter(void *data, SDL_Event *event) {
    Uint32 type = event->type;
    switch (type) {
/**
* @bug according to SDL document, this function may be called in different thread
*      but the SDL renderer function can't be called in other thread but the main thread.
*      On window, I call this because when we change the size, it will not render any more.
*      I guess that if we use dirext3D instead of opengl, we can call the render function in different thread.
*      Actually, it works normally but it was undefined.
*      this problem is also happen when we move the window on windows, which means that if we move the window, the render will stop
*/
    case SDL_WINDOWEVENT:
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && 0      // we close it.
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            WINDOW_MANAGER->patch_event_to(event->window.windowID, *event, sui::Window_manager::window_message_listening);
            return 0;
        }
#endif
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        WINDOW_MANAGER->patch_event_to(event->key.windowID, *event, sui::Window_manager::window_message_listening);
        return 0;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        WINDOW_MANAGER->patch_event_to(event->button.windowID, *event, sui::Window_manager::window_message_listening);
        return 0;
    case SDL_MOUSEWHEEL:
        WINDOW_MANAGER->patch_event_to(event->wheel.windowID, *event, sui::Window_manager::window_message_listening);
        return 0;
    case SDL_MOUSEMOTION:
        WINDOW_MANAGER->patch_event_to(event->motion.windowID, *event, sui::Window_manager::window_message_listening);
        return 0;
    // case Event_type::update_all_window:
    //     WINDOW_MANAGER->update_all_window();
    //     return 0;
    // case Event_type::update_window:
    //     WINDOW_MANAGER->update_window(static_cast<Window_base *>(event->user.data1));
    //     return 0;
    case SDL_QUIT:
        return 1;
    default:
        return 1;
    }
    return 1;
}

int get_event(SDL_Event *event, uint32_t interval) {
    if (run_mode == poll) {
        return SDL_PollEvent(event);
    } else if (run_mode == wait) {
        return SDL_WaitEvent(event);
    } else if (run_mode == wait10) {
        return SDL_WaitEventTimeout(event, 10);
    } else {
        ERR(<< "unkown run mode");
        return 0;
    }
}

void clean() {
    if (clean_last) {
        clean_last();
    }
    DBG(<< "SDL_main quit.");
}