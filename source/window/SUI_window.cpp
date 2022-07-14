#include <cstddef>
#include <memory>
#include <string>
#include <queue>
#include <unordered_map>

#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"

#include "SUI_button.h"
#include "SUI_geometry.h"
#include "SUI_in_canvas.h"
#include "SUI_drawable.h"
#include "SUI_event_handler.h"
#include "SUI_in_events.h"
#include "SUI_in_main.h"
#include "SUI_in_window_data.h"
#include "SUI_in_window_manager.h"
#include "SUI_object.h"
#include "SUI_property.h"
#include "SUI_window.h"
#include "SUI_in_debug.h"
#include "SUI_in_texture_sdl_manager.h"

namespace sui {

// this code is copyed from the internet, which is used to change the transparent of a window
// COLORREF defaultTransparentColor = RGB(255, 0, 255);
// bool windowColorKey(SDL_Window *window, COLORREF colorKey) {
//     SDL_SysWMinfo wmInfo;
//     SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
//     SDL_GetWindowWMInfo(window, &wmInfo);
//     HWND hWnd = wmInfo.info.win.window;
// 
//     // Change window type to layered
//     SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
// 
//     // Set transparency color
//     return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
// }

// debug
// SDL_Window *pw = nullptr;
// SDL_Renderer *pr = nullptr;
// debug

Window::Window(const std::string &title, int width, int height,
               int posX, int posY, Window_flag flag) : Geometry{0, 0, width, height}, Drawable(width, height) {

    object_name = "window";
    // all object except the root should be add to the trash_root initially
    if (TRASH_ROOT->add_node(this) == false) {
        ERR(<< "create window failure\n");
    }
    pData = std::make_unique<Window_data>();
    Uint32 wflag = 0;
    if (flag & Window_flag::window_flag_borderless) {
        wflag |= SDL_WINDOW_BORDERLESS;
    }
    if (flag & Window_flag::window_flag_full_screen) {
        wflag |= SDL_WINDOW_FULLSCREEN;
    }
    if (flag & Window_flag::window_flag_full_screen_desktop) {
        wflag |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (flag & Window_flag::window_flag_hidden) {
        wflag |= SDL_WINDOW_HIDDEN;
    }
    if (flag & Window_flag::window_flag_resizable) {
        wflag |= SDL_WINDOW_RESIZABLE;
    }
    // create the render environment
    pData->pWnd = SDL_CreateWindow(title.c_str(), posX, posY, width, height, wflag | SDL_WINDOW_HIDDEN);
    // because the canvas in Drawable is bind the posX and the posY of the Geometry, and when use window, we should make it zero
    // Geometry::set_posX(0);
    // Geometry::set_posY(0);
    pData->pRenderer = SDL_CreateRenderer(pData->pWnd, -1, 0);
// debug
    // pw = pData->pWnd;
    // pr = pData->pRenderer;
// debug
    // the window manager should control the window, when the program quit, it will close all window
    // and we set false so the window is flag hiden and will not receive anyother message
    WINDOW_MANAGER->add_window(this, Window_manager::window_message_unlistening);
    set_color(0, 0, 0, 255);
    set_background_color(255, 255, 255, 255);
}

Window::Window(const std::string &title, int width, int height, Window_flag flag) :
    Window(title, width, height, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, flag) {}

Window::~Window() {
    /**
    * the canvas's texture should be destroyed before destroy the render and window beacuse there are created by this window and renderer
    */
    /**
    * @todo this process appear three times, restruct it a function
    */
    std::queue<Object*> obj_q;
    obj_q.push(this);
    while (!obj_q.empty()) {
        int size = obj_q.size();
        for (int i = 0; i < size; ++i) {
            Object *pObj = obj_q.front();
            obj_q.pop();
            for (auto p : pObj->get_node_list()) {
                obj_q.push(p);
            }
            Drawable *pd = dynamic_cast<Drawable *>(pObj);
            // may be pd is not a drawable object
            if (pd == nullptr) {
                ERR(<< "a nullptr point from dynamic, may be a object is no drawable");
            }
            if (pd != nullptr) {
                // pd->set_redraw_flag(true);
                pd->destroy_content();
                // pd->draw(canvas);
            }
        }
    }
    // the Window_data will be freed auto
    SDL_DestroyRenderer(pData->pRenderer);
    SDL_DestroyWindow(pData->pWnd);
    DBG(<< get_name() << " SDL window destroy ok in ~Window");
    DBG(<< "delete " <<  get_name() << " ok");
}

void Window::show() {
    if (get_parent() == TRASH_ROOT) {
        // when show, is was added to the root and make the window_manager patch events to the window by setting true flag
        WINDOW_MANAGER->add_window(this, Window_manager::window_message_listening);
        ROOT->add_node(this);
    }
    SDL_ShowWindow(pData->pWnd);
}

void Window::close() {
    DBG(<< get_name() << " (window id:" << get_window_id() << ") close in event circle");
    // trans the object to the trash_root
    ROOT->remove_node(this);
    // remove it in window manager, so that it will not send message to this window
    WINDOW_MANAGER->remove_window(this);
    // currently just hide it simply, it will be destroy when program quit or clean the trash_root
    SDL_HideWindow(pData->pWnd);
}

void Window::set_window_title(const std::string &title) {
    SDL_SetWindowTitle(pData->pWnd, title.c_str());
}

// ovrride the function so that it can return the positon on desktop
int Window::get_posX() const {
    int x = 0;
    SDL_GetWindowPosition(pData->pWnd, &x, nullptr);
    return x;
}

int Window::get_posY() const {
    int y = 0;
    SDL_GetWindowPosition(pData->pWnd, nullptr, &y);
    return y;
}

void Window::set_posX(int x) {
    int y = get_posY();
    SDL_SetWindowPosition(pData->pWnd, x, y);
}

void Window::set_posY(int y) {
    int x = get_posX();
    SDL_SetWindowPosition(pData->pWnd, x, y);
}

void Window::set_position(int x, int y) {
    SDL_SetWindowPosition(pData->pWnd, x, y);
}

/**
* @warning when the widnow size changed, you should update the width and height property which in drawable in event deal function
*          instead of in these two function below as when you change the size of the window through these functions, it will product RESIZE event too
*/
// override to make the window width change
void Window::set_width(int w) {
    int h = get_height();
    SDL_SetWindowSize(pData->pWnd, w, h);
    // update not here
    // Drawable::set_width(w);
}

void Window::set_height(int h) {
    int w = get_width();
    SDL_SetWindowSize(pData->pWnd, w, h);
    // update not here
    // Drawable::set_height(h);
}

void Window::set_size(int w, int h) {
    SDL_SetWindowSize(pData->pWnd, w, h);
    // Drawable::set_width(w);
    // Drawable::set_height(h);
}

// int Window::get_window_width() {
//     int w = 0;
//     // SDL_GetWindowSize(pData->pWnd, &w, nullptr);
//     w = get_width();
//     return w;
// }

// int Window::get_window_height() {
//     int h = 0;
//     // SDL_GetWindowSize(pData->pWnd, nullptr, &h);
//     h = get_height();
//     return h;
// }

Uint32 Window::get_window_id() const {
    return SDL_GetWindowID(pData->pWnd);
}

void Window::update_all_with_children() {
    // we will no change the object tree
    std::queue<Object*> obj_q;
    obj_q.push(this);
    while (!obj_q.empty()) {
        int size = obj_q.size();
        for (int i = 0; i < size; ++i) {
            Object *pObj = obj_q.front();
            obj_q.pop();
            for (auto p : pObj->get_node_list()) {
                obj_q.push(p);
            }
            // may be some object on the object tree is not drawable
            Drawable *pd = dynamic_cast<Drawable *>(pObj);
            if (pd == nullptr) {
                ERR(<< "a nullptr point from dynamic, may be a object is no drawable");
            } else{
                pd->set_redraw_flag(true);
            }
        }
    }
}

void Window::deal_window_resized_event(Event &e) {
    DBG(<< get_name() << "(window id:" << e.event.window.windowID << ") resized");
    Drawable::set_width(e.event.window.data1);
    Drawable::set_height(e.event.window.data2);
    // when the window size change, all texture create for this window will be invalid
    // if any object recreate a texture, it copy the origin invalid content to the new texture, it was invalid
    // so every object should redraw anyway.
    TEXTURE_SDL_MANAGER->invalid_texture(pData->pRenderer);
    update_all_with_children();
    redraw();
    DBG(<< get_name() << "(window id:" << e.event.window.windowID << ") resized deal end");
}

void Window::deal_window_close_event(Event &e) {
    close();
}

// draw the window on a canvas
void Window::draw(Canvas &canvas) {
    DBG(<< get_name() << "( window id: " << get_window_id() << ") draw widnow");
    canvas.save_env();
    draw_background(canvas);
    canvas.restore_env();
    DBG(<< get_name() << "( window id: " << get_window_id() << ") draw widnow ok");
}

/**
* @bug may be on high dpi system, the size can draw is different width the window size
*/
void Window::redraw() {
    // int w, h;
    // SDL_GetRendererOutputSize(pData->pRenderer, &w, &h);
    // create a new canvas which has the render information of the window
    // then the canvas will pass to the object the window contains to update the render information
    Canvas canvas{*this, 0, 0, 0, get_width(), get_height(), get_depth()};
    draw_all(canvas);
    // draw all on the window now
    canvas.paint_on_window(*this);
}

void Window::draw_all(Canvas &canvas) {
    canvas.save_env();
    canvas.set_color(255, 255, 255, 0);
    canvas.clear();
    canvas.restore_env();

    std::queue<Object*> obj_q;
    obj_q.push(this);
    while (!obj_q.empty()) {
        int size = obj_q.size();
        for (int i = 0; i < size; ++i) {
            Object *pObj = obj_q.front();
            obj_q.pop();
            for (auto p : pObj->get_node_list()) {
                obj_q.push(p);
            }
            Drawable *pd = dynamic_cast<Drawable *>(pObj);
            // may be pd is not a drawable object
            if (pd == nullptr) {
                ERR(<< "a nullptr point from dynamic, may be a object is no drawable");
            }
            if (pd != nullptr) {
                // pd->set_redraw_flag(true);
                pd->using_buffer_draw(canvas);
                // pd->draw(canvas);
            }
        }
        // the redraw flag is true?
        // ERROR!!! why my data change!
    }
}

/**
* @bug when the window patch events to its children, the child should not deletable anything object, otherwise it will may cause hand point
*/
void Window::deal_key_down_event(Keyboard_event &key_event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        // use dynamic_cast here because the object is multi-inherit
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_down_event(e_p, key_event);
    }
}

void Window::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_down_event(e_p, mouse_button);
    }
}

void Window::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_up_event(e_p, mouse_button);
    }
}

void Window::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_wheel_event(e_p, mouse_wheel);
    }
}

void Window::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_move_event(e_p, mouse_motion);
    }
}

void Window::deal_key_up_event(Keyboard_event &key_event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_up_event(e_p, key_event);
    }
}

void Window::deal_other_event(Event &event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_other_event(e_p, event);
    }
}
}