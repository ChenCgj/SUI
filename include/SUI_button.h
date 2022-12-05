
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
    enum class Button_event {
        be_down, be_up, be_move_out, be_move_in, be_move
    };
    Button(const std::string &title = "Button", int x = 0, int y = 0, int w = 60, int h = 30);
    void add_listener(const std::function<void (const Mouse_motion_event &, void *)> &func, Button_event event, void *arg);
    void add_listener(const std::function<void (const Mouse_button_event &, void *)> &func, Button_event event, void *arg);
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
    std::string title;
    int in_button;
    std::function<void (const Mouse_motion_event &, void *)> cb_move_out, cb_move_in, cb_move;
    std::function<void (const Mouse_button_event &, void *)> cb_down, cb_up;
    void *a_down, *a_up, *a_move_out, *a_move_in, *a_move;
    // Element_status statu;
    friend class Event_handler_helper;
};
}
#endif