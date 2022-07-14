#include "SUI_element.h"
#include "SUI_geometry.h"
#include "SUI_in_canvas.h"
#include "SUI_drawable.h"
#include "SUI_in_events.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"
#include "SUI_main.h"
namespace sui {

Element::Element(int posX, int posY, int width, int height) : Geometry{posX, posY, width, height}, Drawable(posX, posY, width, height) {
    object_name = "Element";
    // we should add the sub object to the trash root until it was add to a parent
    if (!TRASH_ROOT->add_node(this)) {
        ERR(<< "Element added to root failure.");
    }
    statu = Element_status::None;
}

// simply draw a rect contain the element
void Element::draw(Canvas &canvas) {
    DBG(<< "draw element...");
    canvas.save_env();
    canvas.set_color(255, 255, 255, 255);
    canvas.draw_rect(Rect{0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())});
    canvas.restore_env();
}

Element::~Element() = default;

void Element::deal_key_down_event(Keyboard_event &key_event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        // use dynamic_cast here because the object is multi-inherit
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_down_event(e_p, key_event);
        if (key_event.handle()) {
            return;
        }
    }
}

void Element::deal_key_up_event(Keyboard_event &key_event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_up_event(e_p, key_event);
        if (key_event.handle()) {
            return;
        }
    }
}

void Element::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_down_event(e_p, mouse_button);
        if (mouse_button.handle()) {
            break;
        }
    }
}

void Element::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_up_event(e_p, mouse_button);
        if (mouse_button.handle()) {
            return;
        }
    }
}

void Element::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_wheel_event(e_p, mouse_wheel);
        if (mouse_wheel.handle()) {
            return;
        }
    }
}

void Element::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_move_event(e_p, mouse_motion);
        if (mouse_motion.handle()) {
            return;
        }
    }
}

void Element::deal_other_event(Event &event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_other_event(e_p, event);
        if (event.handle()) {
            return;
        }
    }
}
}