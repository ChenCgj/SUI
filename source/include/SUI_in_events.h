/**
* @file SUI_in_events.h
* @brief a repack of SDL_Event
* @todo enrich these event strct and simplfiy the event deal function
*/

#ifndef SUI_IN_EVENTS_H
#define SUI_IN_EVENTS_H

#include "SDL_events.h"
namespace sui {
struct Key_board_event {
    SDL_KeyboardEvent event;
};

struct Text_editint_event {
    SDL_TextEditingEvent event;
};

struct Text_input_event {
    SDL_TextInputEvent event;
};

struct Mouse_motion_event {
    SDL_MouseMotionEvent event;
};

struct Mouse_button_event {
    SDL_MouseButtonEvent event;
};

struct Mouse_wheel_event {
    SDL_MouseWheelEvent event;
};

struct Event {
    SDL_Event event;
};
}
#endif