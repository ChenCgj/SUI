#include "SUI_in_canvas.h"
#include "SUI_in_events.h"
#include "SUI_in_debug.h"
#include "SUI_tool.h"
#include "SUI_image.h"
#include "SUI_button.h"

namespace sui {
Button::Button(const std::string &title, int x, int y, int w, int h) : Geometry{x, y, w, h}, Element(x, y, w, h), callback{nullptr}, in_button{-1} {
    object_name = "button";
    this->title = title;
    statu = Element_status::button_normal;
}

void Button::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw button start...");
    canvas.save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    Color color = {0, 0, 0, 255};
    Rect r = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    canvas.draw_text(r, title, "consola.ttf", color, 18);
    canvas.restore_env();
    DBG(<< get_name() << "draw button ok");
}

void Button::add_listener(std::function<void (void)> func, Button_event event) {
    this->callback[event] = func;
}

Button::~Button() {
    DBG(<< get_name() << "was destroy.");
}

void Button::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    Element::deal_mouse_button_down_event(mouse_button);
    if (mouse_button.handle()) {
        return;
    }
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        statu = button_press;
 
        if (callback[down]) {
            callback[down]();
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Button::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    Element::deal_mouse_button_up_event(mouse_button);
    if (mouse_button.handle()) {
        return;
    }
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        statu = button_normal;
        
        if (callback[up]) {
            callback[up]();
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Button::deal_mouse_move_event(Mouse_motion_event &move_event) {
    Element::deal_mouse_move_event(move_event);
    if (move_event.handle()) {
        return;
    }
    bool leave_out = false;
    bool leave_in = false;
    int mouse_x = move_event.get_pos().first;
    int mouse_y = move_event.get_pos().second;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {

        if (statu != Element_status::button_press) {
            statu = Element_status::button_hover;
        }

        if (in_button == 0) {
            leave_in = true;
        }
        in_button = 1;

        if (callback[move]) {
            callback[move]();
            // move_event.set_handle(true);
        }
        set_redraw_flag(true);
        present_all();
    } else {
        if (in_button == 1) {
            leave_out = true;
        }
        in_button = 0;
    }
    if (leave_in) {
        deal_mouse_move_in(move_event);
    }
    if (leave_out) {
        deal_mouse_move_out(move_event);
    }
}

void Button::deal_mouse_move_out(Mouse_motion_event &move_event) {
    statu = Element_status::button_normal;
    if (callback[move_out]) {
        callback[move_out]();
    }
    set_redraw_flag(true);
    present_all();
}

void Button::deal_mouse_move_in(Mouse_motion_event &move_event) {
    if (statu != Element_status::button_press) {
        statu = Element_status::button_hover;
    }
    if (callback[move_in]) {
        callback[move_in]();
    }
    set_redraw_flag(true);
    present_all();
}

void Button::draw_border(Canvas &canvas, Element_status statu) {
    uint8_t r, g, b, a;
    get_color(r, g, b, a, statu);
    canvas.set_color(r, g, b, a);
    Rect rect = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    double radius = std::min(get_width() / 4, get_height() / 4);
    Round_rect rrect{rect, radius};
    canvas.draw_shape(rrect);
}

void Button::draw_background(Canvas &canvas, Element_status statu) {
    uint8_t r, g, b, a;
    get_background_color(r, g, b, a, statu);
    canvas.set_color(r, g, b, a);
    Rect rect = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    double radius = std::min(get_width() / 4.0, get_height() / 4.0);
    Round_rect rrect{rect, radius};
    canvas.fill_shape(rrect);
    Image *bg = get_background_image(statu);
    static bool flag = true;
    // it's ok to only set the mask one time for we will use this mask forever
    if (bg && flag) {
        Graphic_board_base graphic_board{bg->get_width(), bg->get_height()};
        graphic_board.set_draw_callback(std::function<void (Graphic_board_base *)>([=](Graphic_board_base *arg) {
            arg->set_draw_color(Color{0, 0, 0, 0}, true);
            arg->clear(true);
            arg->set_draw_color(Color{255, 255, 255, 255}, true);
            Rect rect = {0, 0, static_cast<double>(this->get_width()), static_cast<double>(this->get_height())};
            double radius = std::min(this->get_width() / 4.0, this->get_height() / 4.0);
            Round_rect rrect{rect, radius};
            arg->fill_shape(Round_rect{rect, radius}, true);
        }));
        /**
        * @bug when load new mask, this bg's board should be update
        */
        bg->load_mask(graphic_board);
        flag = false;
    }
    if (bg) {
        bg->draw_image(canvas, 0, 0);
    }
    // Ellipse_arc ea(Point{get_width() / 2.0, get_height() / 2.0}, 20, 10, 0, 8 * atan(1));
    // canvas.draw_shape(ea);
}
}