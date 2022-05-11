#include <functional>
#include <stdint.h>
#include <stdio.h>

#include "SUI_button.h"
#include "SUI_element.h"
#include "SUI_drawable.h"
#include "SUI_geometry.h"
#include "SUI_in_events.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"

namespace sui {
Button::Button(int x, int y, int w, int h) : Element(x, y, w, h) {
    object_name = "button";
    set_background_color(0, 0, 0, 0);
    set_color(0, 0, 0, 255);
    callback = nullptr;
}

void Button::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw button start...");
    canvas.save_env();
    uint8_t r, g, b, a;
    get_background_color(r, g, b, a);
    canvas.set_color(r, g, b, a);
    canvas.fill_rect(Rect{0, 0, get_width(), get_height()});
    get_color(r, g, b, a);
    canvas.set_color(r, g, b, a);
    canvas.draw_rect(Rect{0, 0, get_width(), get_height()});
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
        
        if (callback) {
            callback();
        }
    }
}

void Button::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {

}

void Button::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    
}

void Button::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {

}

void Button::deal_other_event(Event &event) {

}

}