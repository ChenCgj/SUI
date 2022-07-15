#include "SUI_event_handler.h"
#include "SUI_in_event_handler_helper.h"
#include "SUI_window.h"

namespace sui {

Event_handler_helper::Event_handler_helper() = default;
Event_handler_helper::~Event_handler_helper() = default;

const Event_handler_helper *Event_handler_helper::instance() {
    static Event_handler_helper helper;
    return &helper;
}

void Event_handler_helper::deal_key_down_event(Event_handler* handler, Keyboard_event &key_event) const {
    handler->deal_key_down_event(key_event);
}

void Event_handler_helper::deal_key_up_event(Event_handler* handler, Keyboard_event &key_event) const {
    handler->deal_key_up_event(key_event);
}

void Event_handler_helper::deal_mouse_button_down_event(Event_handler* handler, Mouse_button_event &mouse_button) const {
    handler->deal_mouse_button_down_event(mouse_button);
}

void Event_handler_helper::deal_mouse_button_up_event(Event_handler* handler, Mouse_button_event &mouse_button) const {
    handler->deal_mouse_button_up_event(mouse_button);
}

void Event_handler_helper::deal_mouse_wheel_event(Event_handler* handler, Mouse_wheel_event &mouse_wheel) const {
    handler->deal_mouse_wheel_event(mouse_wheel);
}

void Event_handler_helper::deal_mouse_move_event(Event_handler* handler, Mouse_motion_event &mouse_motion) const {
    handler->deal_mouse_move_event(mouse_motion);
}

void Event_handler_helper::deal_other_event(Event_handler* handler, Event &event) const {
    handler->deal_other_event(event);
}

void Event_handler_helper::deal_window_close_event(Window *pWindow, Event &window_event) const {
    pWindow->deal_window_close_event(window_event);
}
void Event_handler_helper::deal_window_resized_event(Window *pWindow, Event &window_event) const {
    pWindow->deal_window_resized_event(window_event);
}
}