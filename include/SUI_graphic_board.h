/**
* @file SUI_graphic_board.h
* @brief provide a board for drawing
*/

#ifndef SUI_GRAPHIC_BOARD
#define SUI_GRAPHIC_BOARD

#include <functional>
#include "SUI_color.h"
#include "SUI_element.h"
#include "SUI_event_handler.h"
#include "SUI_graphic_board_base.h"
#include "SUI_main.h"
#include "SUI_shape.h"

namespace sui {
class Graphic_board : public Graphic_board_base, public Element {
public:
    Graphic_board(int posX, int posY, int width, int height);
    void draw(Canvas &canvas) override;
    ~Graphic_board();
private:
    // void deal_key_down_event(Keyboard_event &key_event) override;
    // void deal_key_up_event(Keyboard_event &key_event) override;
    // void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    // void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    // void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    // void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    // void deal_other_event(Event &event) override;
    // Element_status statu;
    // std::function<void (void *arg)> free_arg;
    // void *arg;
    // bool delete_arg;
    friend class Event_handler_helper;
};
}
#endif