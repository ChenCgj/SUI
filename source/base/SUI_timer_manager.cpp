#include "SUI_in_timer_manager.h"

namespace sui {

Timer_manager::Timer_manager() {}

int Timer_manager::add_timer(Uint32 interval, SDL_TimerCallback callback, void *param) {
    int id = SDL_AddTimer(interval, callback, param);
    IDs.insert(id);
    return id;
}

void Timer_manager::remove_timer(int id) {
    SDL_RemoveTimer(id);
    IDs.erase(id);
}

Timer_manager::~Timer_manager() {
    for (auto iter = IDs.begin(); iter != IDs.end(); ++iter) {
        SDL_RemoveTimer(*iter);
    }
}

Timer_manager *Timer_manager::instance() {
    static Timer_manager *tm = new Timer_manager();
    return tm;
}
}