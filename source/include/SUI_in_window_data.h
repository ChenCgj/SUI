#include "SDL_render.h"
#include "SDL_events.h"
#include "SDL_thread.h"
#include "SUI_in_window_manager.h"
#include "SUI_window.h"
namespace sui {
struct Window_data {
    SDL_Window *pWnd;
    SDL_Renderer *pRenderer;
    Window_manager::Window_listen_status listen_statu;
    long long texture_count;
    bool destroyable;
};
}