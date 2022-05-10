#include <cstddef>
#include <memory>
#include <string>
#include <queue>
#include <unordered_map>

#include "SDL_events.h"
#include "SDL_mutex.h"
#include "SDL_render.h"
#include "SDL_video.h"

#include "SUI_button.h"
#include "SUI_canvas.h"
#include "SUI_drawable.h"
#include "SUI_event_handler.h"
#include "SUI_in_events.h"
#include "SUI_in_main.h"
#include "SUI_in_window_data.h"
#include "SUI_in_window_manager.h"
#include "SUI_object.h"
#include "SUI_property.h"
#include "SUI_window.h"
#include "SUI_in_canvas_data.h"
#include "SUI_in_debug.h"

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

// this lock is aim to lock the message queue the window deal with
std::unordered_map<Window*, std::pair<SDL_mutex*, SDL_cond*>> window_message_queue_lock_map;
// this lock is use to lock the mutex_map above which lock the message queue, will be destroyed by last window
SDL_mutex *lock_window_message_queue_lock_map = SDL_CreateMutex();

/**
* @todo this function should be static, but friend function can't set to static, avoid friend function!
*/
// use to deal event in a new thread
/*static */int deal_event(void *pWindow);

Window::Window(const std::string &title, int width, int height,
               int posX, int posY, Window_flag flag) : Drawable(width, height) {

    object_name = "window";
    SDL_mutex *m = SDL_CreateMutex();
    if (m == nullptr) {
        ERR(<< "create mutex failure when create window");
    }
    SDL_cond *c = SDL_CreateCond();
    if (c == nullptr) {
        ERR(<< "create condition variable failure when create window");
    }
    if (m && c) {
        SDL_LockMutex(lock_window_message_queue_lock_map);
        window_message_queue_lock_map[this] = std::make_pair(m, c);
        SDL_UnlockMutex(lock_window_message_queue_lock_map);
    }
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
    pData->pRenderer = SDL_CreateRenderer(pData->pWnd, -1, 0);
    // the window manager should control the window, when the program quit, it will close all window
    // and we set false so the window is flag hiden and will not receive anyother message
    WINDOW_MANAGER->add_window(this, false);
    set_color(0, 0, 0, 255);
    set_background_color(255, 255, 255, 255);
    // create a new thread
    pData->event_deal_thread = SDL_CreateThread(sui::deal_event, "deal window event thread", this); 
}

Window::Window(const std::string &title, int width, int height, Window_flag flag) :
    Window(title, width, height, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, flag) {}

Window::~Window() {
    SDL_WaitThread(pData->event_deal_thread, nullptr);
    pData->event_deal_thread = nullptr;
    // the Window_data will be freed auto
    SDL_DestroyRenderer(pData->pRenderer);
    SDL_DestroyWindow(pData->pWnd);
    DBG(<< get_name() << " SDL window destroy ok in ~Window");

    bool should_delete_lock = false;
    SDL_LockMutex(lock_window_message_queue_lock_map);
    SDL_mutex *m = window_message_queue_lock_map[this].first;
    SDL_cond *c = window_message_queue_lock_map[this].second;
    window_message_queue_lock_map.erase(this);
    // if this is the last window
    should_delete_lock = window_message_queue_lock_map.empty();
    SDL_UnlockMutex(lock_window_message_queue_lock_map);
    if (m) {
        SDL_DestroyMutex(m);
        m = nullptr;
    }
    if (c) {
        SDL_DestroyCond(c);
        c = nullptr;
    }
    if (should_delete_lock) {
        SDL_DestroyMutex(lock_window_message_queue_lock_map);
        lock_window_message_queue_lock_map = nullptr;
        DBG(<< "delete lock_window_message_queue_lock_map OK");
    }

    DBG(<< "delete " <<  get_name() << " ok");
}

void Window::show() {
    if (get_parent() == TRASH_ROOT) {
        // when show, is was added to the root and make the window_manager patch events to the window by setting true flag
        WINDOW_MANAGER->add_window(this, true);
        ROOT->add_node(this);
    }
    SDL_ShowWindow(pData->pWnd);
}

void Window::close() {
    static SDL_Event e;
    e.window.type = SDL_WINDOWEVENT;
    e.window.windowID = get_window_id();
    e.window.event = SDL_WINDOWEVENT_CLOSE;
    WINDOW_MANAGER->patch_event_to(get_window_id(), e, Window_manager::window_status_all);
}

void Window::set_window_title(const std::string &title) {
    SDL_SetWindowTitle(pData->pWnd, title.c_str());
}

// ovrride the function so that it can return the positon on desktop
int Window::get_posX() {
    int x = 0;
    SDL_GetWindowPosition(pData->pWnd, &x, nullptr);
    return x;
}

int Window::get_posY() {
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

int Window::deal_event() {

    SDL_LockMutex(lock_window_message_queue_lock_map);
    SDL_mutex *m = window_message_queue_lock_map[this].first;
    SDL_cond *c = window_message_queue_lock_map[this].second;
    SDL_UnlockMutex(lock_window_message_queue_lock_map);

    bool flag = true;
    while (flag) {
        SDL_LockMutex(m);
        if (pData->event_queue.empty()) {
            SDL_CondWait(c, m);
        }
        while (flag && !pData->event_queue.empty()) {
            SDL_Event event = pData->event_queue.front();
            pData->event_queue.pop();
            switch (event.type) {
            case SDL_KEYDOWN: {
                Key_board_event k_e{event.key};
                deal_key_down_event(k_e);
                break;
            }
            case SDL_KEYUP: {
                Key_board_event k_e{event.key};
                deal_key_up_event(k_e);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                Mouse_button_event m_e{event.button};
                deal_mouse_button_down_event(m_e);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                Mouse_button_event m_e{event.button};
                deal_mouse_button_up_event(m_e);
                break;
            }
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    DBG(<< get_name() << " (window id:" << get_window_id() << ") close in event circle");
                    // should trans the object to the trash_root first then remove the window in window_manager
                    // because when the manager is empty, it will make the program quit and destroy the object_tree_lock,
                    // but maybe the window still on the ROOT, in which need the lock to trans to TRASH_ROOT
                    // trans the object to the trash_root
                    ROOT->remove_node(this);
                    // remove it in window manager, so that it will not send message to this window
                    WINDOW_MANAGER->remove_window(this);
                    // currently just hide it simply, it will be destroy when program quit or clean the trash_root
                    SDL_HideWindow(pData->pWnd);
                    flag = false;
                    break;
                case SDL_WINDOWEVENT_EXPOSED:
                    // window need to redraw
                    // because the render should be done in main thread, we push the event to the main thread
                    WINDOW_MANAGER->update_window(this);
                    break;
                case SDL_WINDOWEVENT_RESIZED: {
                    // NOT USED
                    // the event here was never used as we filter the event in SUI_main.cpp:main()
                    DBG(<< get_name() << "(window id:" << get_window_id() << ") resized");
                    ERR(<< "never used??");
                    // we should not render, create window or accept the message in other thread excpet the main thread
                    // make all redraw flag true there
                    // update the size
                    Drawable::set_width(event.window.data1);
                    Drawable::set_height(event.window.data2);
                    // we should update all texture
                    update_all_with_children();
                    break;
                }
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        SDL_UnlockMutex(m);
    }
    return 0;
}

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

int deal_event(void *pWindow) {
    Window *pw = (Window *)(pWindow);
    int ret_value = pw->deal_event();
    // when the event thread end, this window can be deleted
    // and the user should not use the window any more
    pw->destroy(true);
    DBG(<< pw->get_name() << "( " << "window id: " << pw->get_window_id() << ") deal event thread end");
    return ret_value;
}

void Window::deal_window_resized_event(Event &e) {
    Drawable::set_width(e.event.window.data1);
    Drawable::set_height(e.event.window.data2);
    update_all_with_children();
}

// draw the window on a canvas
void Window::draw(Canvas &canvas) {
    DBG(<< get_name() << "( window id: " << get_window_id() << ") draw widnow");
    canvas.save_env();
    uint8_t r, g, b, a;
    get_background_color(r, g, b, a);
    canvas.set_color(r, g, b, a);
    canvas.fill_rect(Rect{0, 0, get_width(), get_height()});
    canvas.restore_env();
}

/**
* @bug may be on high dpi system, the size can draw is different width the window size
*/
void Window::redraw_all() {
    // int w, h;
    // SDL_GetRendererOutputSize(pData->pRenderer, &w, &h);
    // create a new canvas which has the render information of the window
    // then the canvas will pass to the object the window contains to update the render information
    Canvas canvas{*this, 0, 0, 0, get_width(), get_height(), get_depth()};
    draw_all(canvas);
}

void Window::draw_all(Canvas &canvas) {
    canvas.save_env();
    canvas.set_color(255, 255, 255, 255);
    canvas.clear();
    canvas.restore_env();
    std::queue<Object*> obj_q;
    // we will not change the object tree
    obj_q.push(this);

#ifdef __WIN32__
    /**
    * @bug On widnow when the num of children more than 4, the buffer_canvas of the window can't draw on it when the window size change
    *      it need to draw again on the buffer_canvas so that it can draw
    *      and one of the child was the same
    */
    this->set_redraw_flag(true); // can't fixed the bug, but make the window can be drawed after stoping the size changing, during the change, it's incorrect
#endif

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
    // draw all on the window now
    canvas.paint_on_window(*this);
}

/**
* @bug when the window patch events to its children, the child should not deletable anything object, otherwise it will may cause hand point
*/
void Window::deal_key_down_event(Key_board_event &key_event) {
    for (auto p : get_node_list()) {
        // use dynamic_cast here because the object is multi-inherit
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_key_down_event(key_event);
    }
}

void Window::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_mouse_button_down_event(mouse_button);
    }
}

void Window::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_mouse_button_up_event(mouse_button);
    }
}

void Window::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_mouse_wheel_event(mouse_wheel);
    }
}

void Window::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_mouse_move_event(mouse_motion);
    }
}

void Window::deal_key_up_event(Key_board_event &key_event) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_key_up_event(key_event);
    }
}

void Window::deal_other_event(Event &event) {
    for (auto p : get_node_list()) {
        Event_handler *e_p = dynamic_cast<Event_handler *>(p);
        e_p->deal_other_event(event);
    }
}
}