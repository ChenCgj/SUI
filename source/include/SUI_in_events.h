/**
* @file SUI_in_events.h
* @brief a repack of SDL_Event
* @todo enrich these event strct and simplfiy the event deal function
*/

#ifndef SUI_IN_EVENTS_H
#define SUI_IN_EVENTS_H

#include <utility>

#include "SDL_events.h"

#include "SUI_mouse_button.h"
#include "SUI_key_code.h"

namespace sui {

class Event {
public:
    Event(const SDL_Event &e);
    void set_handle(bool handle);
    bool handle() const;
    SDL_Event event;
private:
    bool has_been_handle;
};

class Keyboard_event : public Event {
public:
    Keyboard_event(const SDL_Event &e);
    Key_code get_key() const;
    bool shift() const;
    bool ctrl() const;
    bool alt() const;
    bool capslock() const;
    bool repeat() const;
    bool key_pressed() const;
    bool keydown() const;
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
    bool left_button_down() const;
    bool right_button_down() const;
    bool middle_button_down() const;
    std::pair<int, int> get_pos() const;
    std::pair<int, int> get_relative_pos() const;
private:
};

class Mouse_button_event : public Event {
public:
    Mouse_button_event(const SDL_Event &e);
    Mouse_button get_button() const;
    bool button_down() const;
    int click_count() const;
private:
};

class Mouse_wheel_event : public Event {
public:
    Mouse_wheel_event(const SDL_Event &e);
    float get_scrolled_x() const;
    float get_scrolled_y() const;
private:
};

}
#endif