#include <queue>

#include "glad.h"
#include "SUI_in_canvas.h"
#include "SUI_in_events.h"
#include "SUI_in_debug.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_texture_sdl_manager.h"
#include "SUI_in_event_handler_helper.h"
#include "SUI_in_managers.h"

#include "SUI_window_base.h"

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

bool Window_base::has_create_gl = false;
Window_base::Window_base(const std::string &title, int width, int height,
               int posX, int posY, int flag) : Geometry{0, 0, width, height}, Drawable(width, height), glcontext{nullptr}, args{} {

    object_name = "window_base";
    // all object except the root should be add to the trash_root initially
    if (TRASH_ROOT->add_node(this) == false) {
        ERR(<< "create window_base failure\n");
    }

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
    bool gl = false;    // should create opengl context?
    if (flag & Window_flag::window_flag_opengl && !has_create_gl) {
        wflag |= SDL_WINDOW_OPENGL;
        has_create_gl = true;
        gl = true;
    }
    // create the render environment
    SDL_Window *pWnd = SDL_CreateWindow(title.c_str(), posX, posY, width, height, wflag | SDL_WINDOW_HIDDEN);
    if (!pWnd) {
        ERR(<< "create window fail. SDL_Error: " << SDL_GetError());
        return;
    }
    if (gl) {
        glcontext = SDL_GL_CreateContext(pWnd);
        if (!glcontext) {
            ERR(<< "Create opengl context fail. SDL_Error: " << SDL_GetError());
        } else if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
            ERR(<< "load opengl function failure.");
            SDL_GL_DeleteContext(glcontext);
            glcontext = nullptr;
        }
    }
    // because the canvas in Drawable is bind the posX and the posY of the Geometry, and when use window, we should make it zero
    // Geometry::set_posX(0);
    // Geometry::set_posY(0);
    // the driver index is -1 to make the SDL choose the first one driver which support the flag
    /**
    * @bug On Window, if window size can be changed, we can't choose the opengl because we call SDL render function in SDL_EventFilter in SUI_main.cpp
    */
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWnd, -1, has_create_gl ? SDL_RENDERER_SOFTWARE : SDL_RENDERER_ACCELERATED);
// debug
    // pw = pData->pWnd;
    // pr = pData->pRenderer;
// debug
    // the window manager should control the window, when the program quit, it will close all window
    // and we set false so the window is flag hiden and will not receive anyother message
    id = SDL_GetWindowID(pWnd);
    WINDOW_MANAGER->add_window(this, Window_manager::window_message_unlistening, pWnd, pRenderer);
}

Window_base::Window_base(const std::string &title, int width, int height, int flag) :
    Window_base(title, width, height, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, flag) {}

Window_base::~Window_base() {
    /**
    * the canvas's texture should be destroyed before destroy the render and window beacuse there are created by this window and renderer
    */
    /**
    * @todo this process appear three times, restruct it a function
    */
    // it's unneccessary to call destroy_content() for the SDL_Window and SDL_Renderer will be destroy after all texture are destroyed via window_manager
    // std::queue<Object*> obj_q;
    // obj_q.push(this);
    // while (!obj_q.empty()) {
    //     int size = obj_q.size();
    //     for (int i = 0; i < size; ++i) {
    //         Object *pObj = obj_q.front();
    //         obj_q.pop();
    //         for (auto p : pObj->get_node_list()) {
    //             obj_q.push(p);
    //         }
    //         Drawable *pd = dynamic_cast<Drawable *>(pObj);
    //         // may be pd is not a drawable object
    //         if (pd == nullptr) {
    //             ERR(<< "a nullptr point from dynamic, may be a object is no drawable");
    //         }
    //         if (pd != nullptr) {
    //             // make sure that we have destroy all texture before we destroy the renderer
    //             pd->destroy_content();
    //         }
    //     }
    // }
    // the Window_data will be freed auto
    prepare_destroy();
    DBG(<< "delete " <<  get_name() << " ok");
}

void Window_base::show() {
    if (get_parent() == TRASH_ROOT) {
        // when show, is was added to the root and make the window_manager patch events to the window by setting true flag
        WINDOW_MANAGER->update_window_statu(this, Window_manager::window_message_listening);
        ROOT->add_node(this);
    }
    SDL_ShowWindow(WINDOW_MANAGER->get_sdl_window(this));
}

void Window_base::close() {
    DBG(<< get_name() << " (window id:" << get_window_id() << ") close in event circle");
    // trans the object to the trash_root
    ROOT->remove_node(this);
    // remove it in window manager, so that it will not send message to this window
    WINDOW_MANAGER->remove_window(this);
    // currently just hide it simply, it will be destroy when program quit or clean the trash_root
    SDL_HideWindow(WINDOW_MANAGER->get_sdl_window(this));
    set_destroy(true);
}

void Window_base::set_window_title(const std::string &title) {
    SDL_SetWindowTitle(WINDOW_MANAGER->get_sdl_window(this), title.c_str());
}

// ovrride the function so that it can return the positon on desktop
int Window_base::get_posX() const {
    int x = 0;
    SDL_GetWindowPosition(WINDOW_MANAGER->get_sdl_window(this), &x, nullptr);
    return x;
}

int Window_base::get_posY() const {
    int y = 0;
    SDL_GetWindowPosition(WINDOW_MANAGER->get_sdl_window(this), nullptr, &y);
    return y;
}

void Window_base::set_posX(int x) {
    int y = get_posY();
    SDL_SetWindowPosition(WINDOW_MANAGER->get_sdl_window(this), x, y);
}

void Window_base::set_posY(int y) {
    int x = get_posX();
    SDL_SetWindowPosition(WINDOW_MANAGER->get_sdl_window(this), x, y);
}

void Window_base::set_position(int x, int y) {
    SDL_SetWindowPosition(WINDOW_MANAGER->get_sdl_window(this), x, y);
}

/**
* @warning when the widnow size changed, you should update the width and height property which in drawable in event deal function
*          instead of in these two function below as when you change the size of the window through these functions, it will product RESIZE event too
*/
// override to make the window width change
void Window_base::set_width(int w) {
    int h = get_height();
    SDL_SetWindowSize(WINDOW_MANAGER->get_sdl_window(this), w, h);
    // update not here
    // Drawable::set_width(w);
}

void Window_base::set_height(int h) {
    int w = get_width();
    SDL_SetWindowSize(WINDOW_MANAGER->get_sdl_window(this), w, h);
    // update not here
    // Drawable::set_height(h);
}

void Window_base::set_size(int w, int h) {
    SDL_SetWindowSize(WINDOW_MANAGER->get_sdl_window(this), w, h);
    // Drawable::set_width(w);
    // Drawable::set_height(h);
}

// int Window::get_window_width() {
//     int w = 0;
//     // SDL_GetWindowSize(WINDOW_MANAGER->get_sdl_window(this), &w, nullptr);
//     w = get_width();
//     return w;
// }

// int Window::get_window_height() {
//     int h = 0;
//     // SDL_GetWindowSize(WINDOW_MANAGER->get_sdl_window(this), nullptr, &h);
//     h = get_height();
//     return h;
// }

Uint32 Window_base::get_window_id() const {
    return id;
}

void Window_base::clean_gl_context() {
    if (glcontext) {
        SDL_GL_DeleteContext(glcontext);
        glcontext = nullptr;
    }
}

void Window_base::update_all_with_children() {
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

void Window_base::add_listener(const std::function<void (Keyboard_event &, void *)> &func, Key_event event, void *arg) {
    callback[event] = func;
    args[event] = arg;
}

void Window_base::deal_window_resized_event(Event &e) {
    DBG(<< get_name() << "(window id:" << e.event.window.windowID << ") resized");
    Drawable::set_width(e.event.window.data1);
    Drawable::set_height(e.event.window.data2);
    // when the window size change, all texture create for this window will be invalid
    // if any object recreate a texture, it copy the origin invalid content to the new texture, it was invalid
    // so every object should redraw anyway.
    TEXTURE_SDL_MANAGER->invalid_texture(WINDOW_MANAGER->get_sdl_renderer(this));
    update_all_with_children();
    redraw();
    DBG(<< get_name() << "(window id:" << e.event.window.windowID << ") resized deal end");
}

void Window_base::deal_window_close_event(Event &e) {
    close();
}

// draw the window on a canvas
void Window_base::draw(Canvas &canvas) {
    DBG(<< get_name() << "( window id: " << get_window_id() << ") draw widnow");
    canvas.save_env();
    draw_background(canvas);
    canvas.restore_env();
    DBG(<< get_name() << "( window id: " << get_window_id() << ") draw widnow ok");
}

/**
* @bug may be on high dpi system, the size can draw is different width the window size
*/
void Window_base::redraw() {
    // int w, h;
    // SDL_GetRendererOutputSize(pData->pRenderer, &w, &h);
    // create a new canvas which has the render information of the window
    // then the canvas will pass to the object the window contains to update the render information
    Canvas canvas{*this, 0, 0, 0, get_width(), get_height(), get_depth()};
    draw_all(canvas);
    // draw all on the window now
    canvas.paint_on_window(*this);
}

void Window_base::draw_all(Canvas &canvas) {
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
    }
}

/**
* @bug when the window patch events to its children, the child should not deletable anything object, otherwise it will may cause hand point
*/
void Window_base::deal_key_down_event(Keyboard_event &key_event) {
    if (callback[Key_event::down]) {
        callback[Key_event::down](key_event, args[Key_event::down]);
    }
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        // use dynamic_cast here because the object is multi-inherit
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_down_event(e_p, key_event);
    }
}

void Window_base::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_down_event(e_p, mouse_button);
    }
}

void Window_base::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_button_up_event(e_p, mouse_button);
    }
}

void Window_base::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_wheel_event(e_p, mouse_wheel);
    }
}

void Window_base::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_mouse_move_event(e_p, mouse_motion);
    }
}

void Window_base::deal_key_up_event(Keyboard_event &key_event) {
    if (callback[Key_event::up]) {
        callback[Key_event::up](key_event, args[Key_event::up]);
    }
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_key_up_event(e_p, key_event);
    }
}

void Window_base::deal_other_event(Event &event) {
    std::list<Object *> node_list = get_node_list();
    for (auto p = node_list.rbegin(); p != node_list.rend(); ++p) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(*p);
        HANDLER_HELPER->deal_other_event(e_p, event);
    }
}
}