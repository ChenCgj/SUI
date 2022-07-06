/**
* @file SUI_graphic_board.h
* @brief provide a board for drawing
*/

#ifndef SUI_GRAPHIC_BOARD
#define SUI_GRAPHIC_BOARD

#include <functional>
#include "SUI_canvas.h"
#include "SUI_color.h"
#include "SUI_element.h"
#include "SUI_event_handler.h"
#include "SUI_main.h"
#include "SUI_shape.h"

namespace sui {
class Graphic_board : public Element, public Event_handler {
public:
    Graphic_board(int posX, int posY, int width, int height);
    void set_draw_callback(const std::function<void ()> draw_func/*, void *func_arg, bool delete_arg*/);
    void draw(Canvas &canvas) override;
    void move_to(int x, int y);
    void move(int dx, int dy);
    void line_to(int x, int y);
    void draw_line(int x1, int y1, int x2, int y2);
    void fill();
    void set_color(const Color &color);
    void start_draw_on_image();
    void end_draw_on_image();
    // ~Graphic_board();
private:
    // void deal_key_down_event(Keyboard_event &key_event) override;
    // void deal_key_up_event(Keyboard_event &key_event) override;
    // void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    // void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    // void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    // void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    // void deal_other_event(Event &event) override;
    // Element_status statu;
    std::function<void ()> draw_callback;
    // std::function<void (void *arg)> free_arg;
    // void *arg;
    // bool delete_arg;
    Canvas *pcanvas;
    Point curr_pos;
    friend class Event_handler_helper;
};
}
#endif