#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_in_events.h"
#include "SUI_opengl_graphic.h"
#include "SUI_tool.h"
#include "SUI_utils.h"

namespace sui {

class Opengl_graphic::Opengl_frame_buffer {
public:
    GLuint texture_id, /*depth_rbo, stencil_rbo*/depth_stencil_rbo, fbo;
};

bool Opengl_graphic::init_frame_buffer() {
    glGenFramebuffers(1, &buffer->fbo);
    if (buffer->fbo == 0) {
        ERR(<< "create opengl frame buffer fail. opengl: " << glGetError());
        destroy_frame_buffer();
        return false;
    }
    return update_frame_buffer(get_width(), get_height());
}

void Opengl_graphic::destroy_frame_buffer() {
    if (!buffer) {
        return;
    }
    if (buffer->texture_id) {
        glDeleteTextures(1, &buffer->texture_id);
    }
    if (buffer->depth_stencil_rbo) {
        glDeleteRenderbuffers(1, &buffer->depth_stencil_rbo);
    }
    if (buffer->fbo) {
        glDeleteFramebuffers(1, &buffer->fbo);
    }
    *buffer = {0, 0, 0};
}

bool Opengl_graphic::update_frame_buffer(int width, int height)
{
    if (!buffer || buffer->fbo == 0) {
        return false;
    }
    // glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    if (buffer->texture_id) {
        glDeleteTextures(1, &buffer->texture_id);
    }
    if (buffer->depth_stencil_rbo) {
        glDeleteRenderbuffers(1, &buffer->depth_stencil_rbo);
    }
    buffer->texture_id = buffer->depth_stencil_rbo = 0;

    glGenTextures(1, &buffer->texture_id);
    if (buffer->texture_id == 0) {
        ERR(<< "create opengl texture fail. opengl: " << glGetError());
        destroy_frame_buffer();
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, buffer->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &buffer->depth_stencil_rbo);
    if (buffer->depth_stencil_rbo == 0) {
        ERR(<< "create opengl render buffer fail. opengl: " << glGetError());
        destroy_frame_buffer();
        return false;
    }
    glBindRenderbuffer(GL_RENDERBUFFER, buffer->depth_stencil_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    /**
    * @warning it seems that I only can bind one of depth and stencil?
    * if we bind depth and stencil both, I get a error(GL_FRAMEBUFFER_UNSUPPORTED).
    */
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->depth_rbo);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->stencil_rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->texture_id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->depth_stencil_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ERR(<< "the frame buffer is not completed. opengl: " << glGetError());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        destroy_frame_buffer();
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    return true;
}

Opengl_graphic::Opengl_graphic(int x, int y, int w, int h) : Geometry{x, y, w, h}, Element{x, y, w, h, true},
    buffer{nullptr}, funcs{3, nullptr}, args{3, nullptr},
    cb_button_down{nullptr}, cb_button_up{nullptr}, cb_move{nullptr}, cb_wheel{nullptr} {

    object_name = "opengl_graphic";
    statu = Element_status::None;
    buffer = new Opengl_frame_buffer{0, 0, 0};
    if (!buffer || !init_frame_buffer()) {
        ERR(<< "generate frame buffer fail.");
        return;
    }
    auto update = [=](const int &, void *) -> void {
        update_frame_buffer(get_width(), get_height());
    };
    get_width_property().set_listener(update, nullptr);
    get_height_property().set_listener(update, nullptr);
}

void Opengl_graphic::add_listener(const std::function<void (const Mouse_button_event &, void*)> &func, Opengl_graphic_event event, void *arg) {
    switch (event) {
    case Opengl_graphic_event::oge_button_down:
        cb_button_down = func;
        a_button_down = arg;
        break;
    case Opengl_graphic_event::oge_button_up:
        cb_button_up = func;
        a_button_up = arg;
        break;
    default:
        ERR(<< "Unknow type for Mouse_button_event.");
    }
}

void Opengl_graphic::add_listener(const std::function<void (const Mouse_wheel_event &, void*)> &func, Opengl_graphic_event event, void *arg) {
    switch (event) {
    case Opengl_graphic_event::oge_wheel:
        cb_wheel = func;
        a_wheel = arg;
        break;
    default:
        ERR(<< "Unknow type for Mouse_wheel_event.");
    }
}

void Opengl_graphic::add_listener(const std::function<void (const Mouse_motion_event &, void*)> &func, Opengl_graphic_event event, void *arg) {
    switch (event) {
    case Opengl_graphic_event::oge_move:
        cb_move = func;
        a_move = arg;
        break;
    default:
        ERR(<< "Unknow type for Mouse_motion_event.");
    }
}


void Opengl_graphic::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw opengl_graphic ...");
    if (buffer && buffer->fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);
        if (funcs[1]) {
            funcs[1](args[1]);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        canvas.load_gl_Texture(buffer->texture_id, Rect{0, 0, 0, 0});
    }
    DBG(<< get_name() << "draw opengl_graphic ok.");
}

bool Opengl_graphic::add_opengl_funcs(const std::vector<std::function<void (void *)>> &funcs, const std::vector<void *> args) {
    if (!buffer || !buffer->fbo) {
        ERR(<< "there is not frame buffer.");
        return false;
    }
    if (funcs.size() != 3 || args.size() != 3) {
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);
    if (this->funcs[2]) {
        this->funcs[2](this->args[2]);
    }
    this->funcs = funcs;
    this->args = args;
    if (funcs[0]) {
        funcs[0](args[0]);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

Opengl_graphic::~Opengl_graphic() {
    DBG(<< get_name() << "was destroy.");
    destroy_frame_buffer();
    delete buffer;
    prepare_destroy();
}

// void Opengl_graphic::deal_key_down_event(Keyboard_event &key_event) {

// }

// void Opengl_graphic::deal_key_up_event(Keyboard_event &key_event) {

// }

void Opengl_graphic::deal_mouse_button_down_event(Mouse_button_event &mouse_button) {
    Element::deal_mouse_button_down_event(mouse_button);
    if (mouse_button.handle()) {
        return;
    }
    double mouse_x = mouse_button.event.button.x;
    double mouse_y = mouse_button.event.button.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {
        if (cb_button_down) {
            cb_button_down(mouse_button, a_button_down);
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Opengl_graphic::deal_mouse_button_up_event(Mouse_button_event &mouse_button) {
    Element::deal_mouse_button_up_event(mouse_button);
    if (mouse_button.handle()) {
        return;
    }
    double mouse_x = mouse_button.event.button.x;
    double mouse_y = mouse_button.event.button.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {
        if (cb_button_up) {
            cb_button_up(mouse_button, a_button_up);
        }
        set_redraw_flag(true);
        present_all();
        mouse_button.set_handle(true);
    }
}

void Opengl_graphic::deal_mouse_wheel_event(Mouse_wheel_event &mouse_wheel) {
    Element::deal_mouse_wheel_event(mouse_wheel);
    if (mouse_wheel.handle()) {
        return;
    }
    auto pos = get_mouse_pos();
    double mouse_x = pos.first;
    double mouse_y = pos.second;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {

        if (cb_wheel) {
            cb_wheel(mouse_wheel, a_wheel);
        }
        set_redraw_flag(true);
        present_all();
        mouse_wheel.set_handle(true);
    }
}

void Opengl_graphic::deal_mouse_move_event(Mouse_motion_event &mouse_motion) {
    Element::deal_mouse_move_event(mouse_motion);
    if (mouse_motion.handle()) {
        return;
    }
    double mouse_x = mouse_motion.event.motion.x;
    double mouse_y = mouse_motion.event.motion.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {

        if (cb_move) {
            cb_move(mouse_motion, a_move);
        }
        set_redraw_flag(true);
        present_all();
        mouse_motion.set_handle(true);
    }
}

}