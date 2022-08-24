/**
* @file SUI_window.h
* @brief contains the definition of Window
*/

#ifndef SUI_WINDOW_BASE_H
#define SUI_WINDOW_BASE_H

#include "SUI_drawable.h"
#include "SUI_object.h"
#include "SUI_event_handler.h"
#include "SUI_key_event.h"

namespace sui {
class Canvas;
// use when create window
enum Window_flag {
    window_flag_none = 0x0,
    window_flag_full_screen = 0x1,
    window_flag_full_screen_desktop = 0x10,
    window_flag_hidden = 0x100,
    window_flag_borderless = 0x1000,
    window_flag_resizable = 0x10000
};
/**
* @class Window_base
* @brief window is ususaly created to show ui element and use to contain all element showed on the window
* @warning couldn't create window in thread except the main thread, otherwise the window may show things incorrently
* @warning should call show() to show window after create a window
* @warning when the user close the window, you should not use the window any more, so prepare all before show you window
* @warning all window should be closed, then if a window has not been shown or hide after show, you should call close()
*          when you use this function, you should make sure that you program has another window, otherwise the program will quit
* @todo add listener to achieve that when close the window, make programer can decide whether close the window or not
* @bug when change the size of the window, the space out of range area is black
*/
class Window_base : public Object, public Drawable, public Event_handler {
public:
    Window_base(const std::string &title, int width, int height, int posX, int posY, Window_flag flag = Window_flag::window_flag_none);
    Window_base(const std::string &title, int width, int height, Window_flag flag = Window_flag::window_flag_none);
    virtual ~Window_base();
    void set_window_title(const std::string &title);
    void show();
    void close();
    void draw(Canvas &canvas) override;
    void draw_all(Canvas &canvas) override;
    void redraw();
    void update_all_with_children();
    int get_posX() const override;
    int get_posY() const override;
    void set_posX(int x) override;
    void set_posY(int y) override;
    void set_width(int w) override;
    void set_height(int h) override;
    void set_position(int x, int y);
    void set_size(int w, int h);
    void add_listener(std::function<void (Keyboard_event &)> func, Key_event event);
    uint32_t get_window_id() const;
private:
    void deal_window_resized_event(Event &event);
    void deal_key_down_event(Keyboard_event &key_event) override;
    void deal_key_up_event(Keyboard_event &key_event) override;
    void deal_mouse_button_down_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_button_up_event(Mouse_button_event &mouse_button) override;
    void deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) override;
    void deal_mouse_move_event(Mouse_motion_event &mouse_motion) override;
    void deal_window_close_event(Event &event);
    void deal_other_event(Event &event) override;
    uint32_t id;
    std::function<void (Keyboard_event &)> callback[2];
    friend class Event_handler_helper;
};

} // End of namespace sui
#endif