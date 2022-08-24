#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_in_events.h"
#include "SUI_graphic_board.h"
#include "SUI_tool.h"

namespace sui {

Graphic_board::Graphic_board(int posX, int posY, int width, int height)
    : Geometry(posX, posY, width, height), Graphic_board_base{width, height}, Element(posX, posY, width, height) {
    object_name = "graphic_board";
    // arg = this;
    // delete_arg = false;
    statu = Element_status::graphic_board_normal;
}

Graphic_board::~Graphic_board() = default;

void Graphic_board::set_redraw_flag(bool flag) {
    Drawable::set_redraw_flag(flag);
    if (flag) {
        // if we need update the content when the window's size doesn't change
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

void Graphic_board::add_listener(std::function<void ()> func, Graphic_board_event event) {
    callback[event] = func;
}

void Graphic_board::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    if (mouse_button.handle()) {
        return;
    }
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        
        if (callback[down]) {
            callback[down]();
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
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        
        if (callback[up]) {
            callback[up]();
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
    int mouse_x = mouse_motion.event.button.x;
    int mouse_y = mouse_motion.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        
        if (callback[move]) {
            callback[move]();
        }
        set_redraw_flag(true);
        present_all();
        mouse_motion.set_handle(true);
    }
}
}