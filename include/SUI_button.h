
/**
* @file SUI_button.h
* @brief contains definition of button class
*/

#ifndef SUI_BUTTON_H
#define SUI_BUTTON_H
#include "SUI_event_handler.h"
#include "SUI_element.h"
#include "SUI_decorator.h"
#include <functional>

namespace sui {

/**
* @class Button
* this class was designed to implement button feature.
* however, it draw itself not like a button now
* when the button was click, the funtion which was set by add_listener() will be called
* @warning when you change the button style(such as color), you should call set_redraw_flag(true).
*/

class Button : public Element, public Decorator, public Event_handler {
public:
    Button(int x, int y, int w, int h);
    void add_listener(std::function<void (void)> func);
    void draw(Canvas &canvas) override;
    void deal_key_down_event(Key_board_event &key_event) override;
    void deal_key_up_event(Key_board_event &key_event) override;
    void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    void deal_other_event(Event &event) override;
    ~Button();
private:
    // void *(*callback)(void *);
    std::function<void (void)> callback;
};
}
#endif