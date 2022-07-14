/**
* @file SUI_in_events.h
* @brief a repack of SDL_Event
* @todo enrich these event strct and simplfiy the event deal function
*/

#ifndef SUI_IN_EVENTS_H
#define SUI_IN_EVENTS_H

#include <utility>
#include "SDL_events.h"
#include "SDL_stdinc.h"
#include "SUI_main.h"

namespace sui {

class Event {
public:
    Event(const SDL_Event &e);
    void set_handle(bool handle);
    bool handle();
    SDL_Event event;
private:
    bool has_been_handle;
};

class Keyboard_event : public Event {
public:
    Keyboard_event(const SDL_Event &e);
    Key_code get_key();
    bool shift();
    bool ctrl();
    bool alt();
    bool capslock();
    bool repeat();
    bool key_pressed();
    bool keydown();
private:
};

class Text_editing_event : public Event {
public:
    Text_editing_event(const SDL_Event &e);
private:
};

class Text_input_event : public Event {
public:
    Text_input_event(const SDL_Event &e);
    char *get_text();
private:
};

class Mouse_motion_event : public Event {
public:
    Mouse_motion_event(const SDL_Event &e);
    bool left_button_down();
    bool right_button_down();
    bool middle_button_down();
    std::pair<int, int> get_pos();
    std::pair<int, int> get_relative_pos();
private:
};

class Mouse_button_event : public Event {
public:
    Mouse_button_event(const SDL_Event &e);
    Mouse_button get_button();
    bool button_down();
    int click_count();
private:
};

class Mouse_wheel_event : public Event {
public:
    Mouse_wheel_event(const SDL_Event &e);
    float get_scrolled_x();
    float get_scrolled_y();
private:
};

}
#endif