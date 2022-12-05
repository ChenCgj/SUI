#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_in_events.h"
#include "SUI_graphic_board.h"
#include "SUI_tool.h"

namespace sui {

Graphic_board::Graphic_board(int posX, int posY, int width, int height)
    : Geometry(posX, posY, width, height), Graphic_board_base{width, height}, Element(posX, posY, width, height),
    cb_down{nullptr}, cb_up{nullptr}, cb_move{nullptr}, a_down{nullptr}, a_up{nullptr}, a_move{nullptr} {
    object_name = "graphic_board";
    // arg = this;
    // delete_arg = false;
    statu = Element_status::graphic_board_normal;
}

Graphic_board::~Graphic_board() {
    prepare_destroy();
}

void Graphic_board::set_redraw_flag(bool flag) {
    Drawable::set_redraw_flag(flag);
    if (flag) {
        // flag is true if we need update the content when the window's size doesn't change
        // make graphic_board detect if it doesn't need redraw when flag is false
        set_need_redraw(flag);
    }
}

void Graphic_board::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw graphic board start...");
    canvas.save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    draw_board(canvas);
    canvas.restore_env();

    DBG(<< get_name() << "draw graphic board ok");
}

void Graphic_board::destroy_content() {
    unload_data();
    Element::destroy_content();
}

void Graphic_board::add_listener(const std::function<void (const Mouse_button_event &, void *)> &func, Graphic_board_event event, void *arg) {
    if (event == Graphic_board_event::gbe_down) {
        cb_down = func;
        a_down = arg;
    } else if (event == Graphic_board_event::gbe_up) {
        cb_up = func;
        a_up = arg;
    } else {
        ERR(<< "Unknow type for mouse button event");
    }
}

void Graphic_board::add_listener(const std::function<void (const Mouse_motion_event &, void *)> &func, Graphic_board_event event, void *arg) {
    if (event == Graphic_board_event::gbe_move) {
        cb_move = func;
        a_move = arg;
    } else {
        ERR(<< "Unknow type for mouse button event");
    }
}


void Graphic_board::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    if (mouse_button.handle()) {
        return;
    }
    double mouse_x = mouse_button.event.button.x;
    double mouse_y = mouse_button.event.button.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {
        
        if (cb_down) {
            cb_down(mouse_button, a_down);
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Graphic_board::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    if (mouse_button.handle()) {
        return;
    }
    double mouse_x = mouse_button.event.button.x;
    double mouse_y = mouse_button.event.button.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {

        if (cb_up) {
            cb_up(mouse_button, a_up);
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Graphic_board::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    if (mouse_motion.handle()) {
        return;
    }
    double mouse_x = mouse_motion.event.motion.x;
    double mouse_y = mouse_motion.event.motion.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {

        if (cb_move) {
            cb_move(mouse_motion, a_move);
        }
        set_redraw_flag(true);
        present_all();
        mouse_motion.set_handle(true);
    }
}
}