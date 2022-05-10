#include "SDL_render.h"
#include "SDL_events.h"
#include "SDL_thread.h"
#include "SUI_window.h"
namespace sui {
struct Window::Window_data {
    SDL_Window *pWnd;
    SDL_Renderer *pRenderer;
    std::queue<SDL_Event> event_queue;
    SDL_Thread *event_deal_thread;
};
}