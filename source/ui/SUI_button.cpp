#include <algorithm>
#include <functional>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "SUI_button.h"
#include "SUI_canvas.h"
#include "SUI_element.h"
#include "SUI_drawable.h"
#include "SUI_geometry.h"
#include "SUI_in_events.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"
#include "SUI_main.h"
#include "SUI_shape.h"

namespace sui {
Button::Button(const std::string &title, int x, int y, int w, int h) : Element(x, y, w, h) {
    object_name = "button";
    this->title = title;
    callback = nullptr;
    statu = Element_status::button_normal;
}

void Button::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw button start...");
    canvas.save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    Color color = {0, 0, 0, 255};
    Rect r = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    canvas.draw_text(r, title, "Inkfree.ttf", color, 18);
    canvas.restore_env();
    DBG(<< get_name() << "draw button ok");
}

void Button::add_listener(std::function<void (void)> func) {
    this->callback = func;
}

Button::~Button() {
    DBG(<< get_name() << "was destroy.");
}

void Button::deal_key_down_event(Keyboard_event &key_event) {
    
}

void Button::deal_key_up_event(Keyboard_event &key_event) {

}

void Button::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        statu = button_press;
        
        if (callback) {
            callback();
        }
        set_redraw_flag(true);
        present_all();
    }
}

void Button::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    int mouse_x = mouse_button.event.button.x;
    int mouse_y = mouse_button.event.button.y;
    if (mouse_x < get_width() + get_posX()
        && mouse_x > get_posX()
        && mouse_y < get_height() + get_posY()
        && mouse_y > get_posY()) {
        statu = button_normal;
        

        set_redraw_flag(true);
        present_all();
    }
}

void Button::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    
}

void Button::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {

}

void Button::deal_other_event(Event &event) {

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
    Image *image = get_background_image(statu);
    if (image) {
        canvas.draw_image(*image);
    }
    // Ellipse_arc ea(Point{get_width() / 2.0, get_height() / 2.0}, 20, 10, 0, 8 * atan(1));
    // canvas.draw_shape(ea);
}
}