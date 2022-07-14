#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SUI_in_events.h"
#include "SUI_main.h"
#include <utility>

namespace sui {

Event::Event(const SDL_Event &e) : event{e}, has_been_handle{false} {}

void Event::set_handle(bool handle) {
    has_been_handle = handle;
}

bool Event::handle() {
    return has_been_handle;
}

Keyboard_event::Keyboard_event(const SDL_Event &e) : Event{e} {}

Key_code Keyboard_event::get_key() {
    return static_cast<Key_code>(event.key.keysym.sym);
}

bool Keyboard_event::shift() {
    return event.key.keysym.mod & KMOD_SHIFT;
}

bool Keyboard_event::ctrl() {
    return event.key.keysym.mod & KMOD_CTRL;
}

bool Keyboard_event::alt() {
    return event.key.keysym.mod & KMOD_ALT;
}

bool Keyboard_event::capslock() {
    return event.key.keysym.mod & KMOD_CAPS;
}

bool Keyboard_event::repeat() {
    return event.key.repeat != 0;
}

bool Keyboard_event::key_pressed() {
    return event.key.state == SDL_PRESSED;
}

bool Keyboard_event::keydown() {
    return event.key.type == SDL_KEYDOWN;
}

Text_editing_event::Text_editing_event(const SDL_Event &e) : Event{e} {}


Text_input_event::Text_input_event(const SDL_Event &e) : Event{e} {}

char *Text_input_event::get_text() {
    return event.text.text;
}

Mouse_motion_event::Mouse_motion_event(const SDL_Event &e) : Event{e} {}

bool Mouse_motion_event::left_button_down() {
    return event.motion.state & SDL_BUTTON_LMASK;
}

bool Mouse_motion_event::middle_button_down() {
    return event.motion.state & SDL_BUTTON_MMASK;
}

bool Mouse_motion_event::right_button_down() {
    return event.motion.state & SDL_BUTTON_RMASK;
}

std::pair<int, int> Mouse_motion_event::get_pos() {
    return std::make_pair(event.motion.x, event.motion.y);
}

std::pair<int, int> Mouse_motion_event::get_relative_pos() {
    return std::make_pair(event.motion.xrel, event.motion.yrel);
}

Mouse_button_event::Mouse_button_event(const SDL_Event &e) : Event{e} {}

Mouse_button Mouse_button_event::get_button() {
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

bool Mouse_button_event::button_down() {
    return event.button.state == SDL_PRESSED;
}

int Mouse_button_event::click_count() {
    return event.button.clicks;
}

Mouse_wheel_event::Mouse_wheel_event(const SDL_Event &e) : Event{e} {}

float Mouse_wheel_event::get_scrolled_x() {
    return event.wheel.preciseX;
}

float Mouse_wheel_event::get_scrolled_y() {
    return event.wheel.preciseY;
}
}