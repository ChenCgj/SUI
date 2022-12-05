/**
* @file SUI_graphic_board.h
* @brief provide a board for drawing
*/

#ifndef SUI_GRAPHIC_BOARD
#define SUI_GRAPHIC_BOARD

#include "SUI_element.h"
#include "SUI_graphic_board_base.h"

namespace sui {
class Graphic_board : public Graphic_board_base, public Element {
public:
    enum class Graphic_board_event {
        gbe_down, gbe_up, gbe_move
    };
    Graphic_board(int posX, int posY, int width, int height);
    void draw(Canvas &canvas) override;
    void destroy_content() override;
    void set_redraw_flag(bool flag) override;
    void add_listener(const std::function<void (const Mouse_button_event &, void *)> &func, Graphic_board_event event, void *arg);
    void add_listener(const std::function<void (const Mouse_motion_event &, void *)> &func, Graphic_board_event event, void *arg);
    ~Graphic_board();
private:
    // void deal_key_down_event(Keyboard_event &key_event) override;
    // void deal_key_up_event(Keyboard_event &key_event) override;
    void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    // void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    // void deal_other_event(Event &event) override;
    // Element_status statu;
    // std::function<void (void *arg)> free_arg;
    // void *arg;
    // bool delete_arg;
    // callback
    std::function<void (const Mouse_button_event &, void*)> cb_down, cb_up;
    std::function<void (const Mouse_motion_event &, void*)> cb_move;
    // args
    void *a_down, *a_up, *a_move;
    friend class Event_handler_helper;
};
}
#endif