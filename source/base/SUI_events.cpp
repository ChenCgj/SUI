#include "SUI_in_events.h"

namespace sui {

Event::Event(const SDL_Event &e) : event{e}, has_been_handle{false} {}

void Event::set_handle(bool handle) {
    has_been_handle = handle;
}

bool Event::handle() const {
    return has_been_handle;
}

Keyboard_event::Keyboard_event(const SDL_Event &e) : Event{e} {}

Key_code Keyboard_event::get_key() const {
    return static_cast<Key_code>(event.key.keysym.sym);
}

bool Keyboard_event::shift() const {
    return event.key.keysym.mod & KMOD_SHIFT;
}

bool Keyboard_event::ctrl() const {
    return event.key.keysym.mod & KMOD_CTRL;
}

bool Keyboard_event::alt() const {
    return event.key.keysym.mod & KMOD_ALT;
}

bool Keyboard_event::capslock() const {
    return event.key.keysym.mod & KMOD_CAPS;
}

bool Keyboard_event::repeat() const {
    return event.key.repeat != 0;
}

bool Keyboard_event::key_pressed() const {
    return event.key.state == SDL_PRESSED;
}

bool Keyboard_event::keydown() const {
    return event.key.type == SDL_KEYDOWN;
}

Text_editing_event::Text_editing_event(const SDL_Event &e) : Event{e} {}


Text_input_event::Text_input_event(const SDL_Event &e) : Event{e} {}

char *Text_input_event::get_text() {
    return event.text.text;
}

Mouse_motion_event::Mouse_motion_event(const SDL_Event &e) : Event{e} {}

bool Mouse_motion_event::left_button_down() const {
    return event.motion.state & SDL_BUTTON_LMASK;
}

bool Mouse_motion_event::middle_button_down() const {
    return event.motion.state & SDL_BUTTON_MMASK;
}

bool Mouse_motion_event::right_button_down() const {
    return event.motion.state & SDL_BUTTON_RMASK;
}

std::pair<int, int> Mouse_motion_event::get_pos() const {
    return std::make_pair(event.motion.x, event.motion.y);
}

std::pair<int, int> Mouse_motion_event::get_relative_pos() const {
    return std::make_pair(event.motion.xrel, event.motion.yrel);
}

Mouse_button_event::Mouse_button_event(const SDL_Event &e) : Event{e} {}

Mouse_button Mouse_button_event::get_button() const {
    if (event.button.button == SDL_BUTTON_LEFT) {
        return mouse_left_button;
    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        return mouse_middle_button;
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
        return mouse_right_button;
    } else {
        return mouse_unknow_button;
    }
}

bool Mouse_button_event::button_down() const {
    return event.button.state == SDL_PRESSED;
}

int Mouse_button_event::click_count() const {
    return event.button.clicks;
}

Mouse_wheel_event::Mouse_wheel_event(const SDL_Event &e) : Event{e} {}

float Mouse_wheel_event::get_scrolled_x() const {
#if SDL_MAJOR_VERSION >= 2 && SDL_PATCHLEVEL >= 18
    return event.wheel.preciseX;
#else
    return event.wheel.x;
#endif
}

float Mouse_wheel_event::get_scrolled_y() const {
#if SDL_MAJOR_VERSION >= 2 && SDL_PATCHLEVEL >= 18
    return event.wheel.preciseY;
#else
    return event.wheel.y;
#endif
}
}