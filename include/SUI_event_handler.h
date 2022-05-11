/**
* @file SUI_event_handler.h
* @brief contains the definition of class Event_handler
*/

#ifndef SUI_EVENT_HANDLER_H
#define SUI_EVENT_HANDLER_H
#include "SUI_events.h"

namespace sui {

/**
* @class Event_handler
* @brief every object which want to deal event should be derived from this class
* @warning you should not make the object deletable when dealing events
*/
class Event_handler {
public:
    Event_handler();
    virtual ~Event_handler();
    virtual void deal_key_down_event(Keyboard_event &key_event); // = 0;
    virtual void deal_key_up_event(Keyboard_event &key_event); // = 0;
    virtual void deal_mouse_button_down_event(Mouse_button_event &mouse_button); // = 0;
    virtual void deal_mouse_button_up_event(Mouse_button_event &mouse_button); // = 0;
    virtual void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel); // = 0;
    virtual void deal_mouse_move_event(Mouse_motion_event &mouse_motion); // = 0;
    virtual void deal_other_event(Event &event); // = 0;
};
}
#endif