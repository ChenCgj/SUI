/**
* @file SUI_in_event_handler_helper.h
* @brief contains the definiton of the handler helper
*/

#ifndef SUI_IN_EVENT_HANDLER_HELPER
#define SUI_IN_EVENT_HANDLER_HELPER
#include "SUI_events.h"
namespace sui {
class Event_handler;
class Window_base;
class Event_handler_helper {
public:
    static const Event_handler_helper *instance();
    void deal_key_down_event(Event_handler* handler, Keyboard_event &key_event) const;
    void deal_key_up_event(Event_handler* handler, Keyboard_event &key_event) const;
    void deal_mouse_button_down_event(Event_handler* handler, Mouse_button_event &mouse_button) const;
    void deal_mouse_button_up_event(Event_handler* handler, Mouse_button_event &mouse_button) const;
    void deal_mouse_wheel_event(Event_handler* handler, Mouse_wheel_event &mouse_wheel) const;
    void deal_mouse_move_event(Event_handler* handler, Mouse_motion_event &mouse_motion) const;
    void deal_other_event(Event_handler* handler, Event &event) const;
    void deal_window_close_event(Window_base *pWindow, Event &window_event) const;
    void deal_window_resized_event(Window_base *pWindow, Event &window_event) const;
    ~Event_handler_helper();
private:
    Event_handler_helper();
};
}

#endif