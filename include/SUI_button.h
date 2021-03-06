
/**
* @file SUI_button.h
* @brief contains definition of button class
*/

#ifndef SUI_BUTTON_H
#define SUI_BUTTON_H

#include "SUI_element.h"

namespace sui {

/**
* @class Button
* this class was designed to implement button feature.
* however, it draw itself not like a button now
* when the button was click, the funtion which was set by add_listener() will be called
* @warning when you change the button style(such as color), you should call set_redraw_flag(true).
*/

class Button : public Element {
public:
    enum Button_event {
        down, up, move_out, move_in, move
    };
    Button(const std::string &title = "Button", int x = 0, int y = 0, int w = 60, int h = 30);
    void add_listener(std::function<void (void)> func, Button_event event);
    void draw(Canvas &canvas) override;
    ~Button();
private:
    void draw_border(Canvas &canvas, Element_status statu = Element_status::normal) override;
    void draw_background(Canvas &canvas, Element_status statu = Element_status::button_normal) override;
    void deal_mouse_button_down_event(Mouse_button_event &key_event) override;
    void deal_mouse_button_up_event(Mouse_button_event &key_event) override;
    void deal_mouse_move_event(Mouse_motion_event &move_event) override;
    void deal_mouse_move_out(Mouse_motion_event &move_event);
    void deal_mouse_move_in(Mouse_motion_event &move_event);
    // void *(*callback)(void *);
    std::function<void (void)> callback[5];
    std::string title;
    int in_button;
    // Element_status statu;
    friend class Event_handler_helper;
};
}
#endif