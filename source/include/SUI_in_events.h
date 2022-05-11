/**
* @file SUI_in_events.h
* @brief a repack of SDL_Event
* @todo enrich these event strct and simplfiy the event deal function
*/

#ifndef SUI_IN_EVENTS_H
#define SUI_IN_EVENTS_H

#include "SDL_events.h"
namespace sui {

class Event {
public: 
    SDL_Event event;
    Event(const SDL_Event &e);
};

class Keyboard_event : public Event {
public:
    Keyboard_event(const SDL_Event &e);
};

class Text_editint_event : public Event {
public:
    Text_editint_event(const SDL_Event &e);
};

class Text_input_event : public Event {
public:
    Text_input_event(const SDL_Event &e);
};

class Mouse_motion_event : public Event {
public:
    Mouse_motion_event(const SDL_Event &e);
};

class Mouse_button_event : public Event {
public:
    Mouse_button_event(const SDL_Event &e);
};

class Mouse_wheel_event : public Event {
public:
    Mouse_wheel_event(const SDL_Event &e);
};

}
#endif