#ifndef SUI_TIMER_H
#define SUI_TIMER_H

#include <unordered_set>
#include "SDL_timer.h"

namespace sui {
class Timer_manager {
public:
    static Timer_manager *instance();
    int add_timer(Uint32 interval, SDL_TimerCallback callback, void *param);
    void remove_timer(int id);
    ~Timer_manager();
private:
    Timer_manager();
    std::unordered_set<int> IDs;
};
}
#endif