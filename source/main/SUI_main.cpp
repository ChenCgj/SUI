#include "SDL.h"
#include "SDL_events.h"
#include "SDL_mutex.h"
#include "SDL_render.h"
#include "SDL_thread.h"
#include "SDL_timer.h"
#include "SDL_video.h"

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
                WINDOW_MANAGER->patch_event_to(event.key.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                WINDOW_MANAGER->patch_event_to(event.motion.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case SDL_WINDOWEVENT:
                WINDOW_MANAGER->patch_event_to(event.window.windowID, event, sui::Window_manager::window_status_showing);
                break;
            case Event_type::window_redraw_event:
                static_cast<Window*>(event.user.data1)->redraw_all();
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
        Window *pWnd = WINDOW_MANAGER->get_window(resized_event->window.windowID);
        if (!pWnd) {
            ERR(<< "a unshow window resize");
            return 0;
        }
        DBG(<< pWnd->get_name() << "(window id:" << resized_event->window.windowID << ") resized");
        Event e{*resized_event};
        pWnd->deal_window_resized_event(e);
        /**
        * @bug the tackle_size_change will be set as a event filter which may run in other thread
        *      however, the render is not support to run in thread which is not MAIN THREAD.
        *      But it work currently
        */
        pWnd->redraw_all();
        return 0;
    }
    return 1;
}

void clean() {
    // we can't make sure that dose any object need the object_tree lock when we prepare to end program
    // but we can make sure that this thread needn't this lock, because it can't change the object tree now
    // however, if user create a new thread(which operate the object tree), he should wait the thread to end and then this thread can end
    SDL_DestroyMutex(lock_object_tree);
    lock_object_tree = nullptr;
    DBG(<< "destroy lock_object_tree ok");
    // this lock should be destroyed by the last window, otherwise when we destroy this lock but a window not be destroy will need this lock will lead to problem
    // SDL_DestroyMutex(lock_window_message_queue_lock_map);
    SDL_Quit();
    DBG(<< "SDL_Quit OK");
}