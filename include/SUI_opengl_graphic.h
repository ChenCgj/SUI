/**
* @file SUI_opengl_graphic.h
* @brief save the opengl framebuffer data in a canvas
*/

#ifndef SUI_OPENGL_GRAPHIC_H
#define SUI_OPENGL_GRAPHIC_H

#include <vector>
#include "SUI_element.h"

namespace sui {

/**
* @class Opengl_graphic
* this class was designed to transform the opengl framebuffer data to canvas
*/
class Opengl_graphic : public Element {
public:
    enum class Opengl_graphic_event {
        oge_key_down, oge_key_up, oge_button_down, oge_button_up, oge_wheel, oge_move
    };
    Opengl_graphic(int x = 0, int y = 0, int w = 100, int h = 100);
    void add_listener(const std::function<void (const Mouse_button_event &, void*)> &func, Opengl_graphic_event event, void *arg);
    void add_listener(const std::function<void (const Mouse_wheel_event &, void*)> &func, Opengl_graphic_event event, void *arg);
    void add_listener(const std::function<void (const Mouse_motion_event &, void*)> &func, Opengl_graphic_event event, void *arg);
    bool add_opengl_funcs(const std::vector<std::function<void (void*)>> &funcs, const std::vector<void *> args);
    ~Opengl_graphic();
    void draw(Canvas &canvas) override;
private:
    class Opengl_frame_buffer;
    Opengl_frame_buffer *buffer;
    bool init_frame_buffer();
    void destroy_frame_buffer();
    bool update_frame_buffer(int width, int height);
    std::vector<std::function<void (void *)>> funcs;
    std::vector<void *> args;
    // void deal_key_down_event(Keyboard_event &key_event) override;
    // void deal_key_up_event(Keyboard_event &key_event) override;
    void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;

    std::function<void (Mouse_button_event &, void*)> cb_button_down;
    std::function<void (Mouse_button_event &, void*)> cb_button_up;
    std::function<void (Mouse_motion_event &, void*)> cb_move;
    std::function<void (Mouse_wheel_event &, void *)> cb_wheel;

    void *a_button_down, *a_button_up, *a_move, *a_wheel;
    friend class Event_handler_helper;
};

}
#endif