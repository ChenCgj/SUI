/**
* @file SUI_element.h
* @brief contains definition of class Element
*/

#ifndef SUI_ELEMENT_H
#define SUI_ELEMENT_H

#include "SUI_drawable.h"
#include "SUI_event_handler.h"
#include "SUI_object.h"

namespace sui {

/**
* @class Element
* @brief usually a ui element should be derived from this class
*/
class Element : public Object, public Drawable, public Event_handler {
public:
    Element(int posX, int posY, int width, int height, bool for_gl_data = false);
    virtual void draw(Canvas &canvas) override;
    virtual ~Element();
    void deal_key_down_event(Keyboard_event &key_event) override;
    void deal_key_up_event(Keyboard_event &key_event) override;
    void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    void deal_other_event(Event &event) override;
    void destroy_content() override;
protected:
    Element_status statu;
};

}
#endif