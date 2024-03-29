#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_drawable.h"
#include "SUI_image.h"

namespace sui {
extern template class Property<int>;
extern template void Property<int>::add_binded<>(Property<int> &property, std::function<int (const int &)> func);

Drawable::Drawable(int posX, int posY, int posZ, int width, int height, int depth, bool for_gl_data)
    : Geometry{posX, posY, posZ, width, height, depth},
    canvas_buffer{new Canvas(posX, posY, posZ, width, height, depth, for_gl_data)} {

    // to detect the size change, the canvas size should bind to the drawable element
    auto orign_value_func = std::function<int (const int &)>([](const int &v){return v;});
    canvas_buffer->get_width_property().bind(get_width_property(), orign_value_func);
    canvas_buffer->get_height_property().bind(get_height_property(), orign_value_func);
    canvas_buffer->get_posX_property().bind(get_posX_property(), orign_value_func);
    canvas_buffer->get_posY_property().bind(get_posY_property(), orign_value_func);
}

Drawable::Drawable(int width, int height, bool for_gl_data)
    : Drawable(0, 0, 0, width, height, 0, for_gl_data) {}

Drawable::Drawable(int posX, int posY, int width, int height, bool for_gl_data)
    : Drawable(posX, posY, 0, width, height, 0, for_gl_data) {}

Drawable::~Drawable() {
    delete canvas_buffer;
}

// when we destroy window, we should call this first to destroy all texture
// if you use another canvas, remmber to rewrite this function and call this function
void Drawable::destroy_content() {
    canvas_buffer->unload_renderer();
    /*if have background, we should unload the background image's canvas's texture*/
}

void Drawable::draw_all(Canvas &canvas) {
    draw(canvas);
}

void Drawable::set_redraw_flag(bool flag) {
    // redraw_flag = flag;
    canvas_buffer->set_need_redraw(flag);
}

bool Drawable::get_redraw_flag() {
    // return redraw_flag;
    return canvas_buffer->check_need_redraw();
}

void Drawable::set_always_redraw(bool flag) {
    canvas_buffer->set_always_redraw(flag);
}

bool Drawable::get_always_redraw() const {
    return canvas_buffer->get_always_redraw();
}

void Drawable::save_buffer(Canvas &canvas) {
    // load the render information
    canvas_buffer->load_renderer(canvas);
    // clean the buffer if it's not for opengl
    if (!canvas_buffer->is_for_opengl()) {
        canvas_buffer->save_env();
        canvas_buffer->set_color(0, 0, 0, 0);
        canvas_buffer->clear();
        canvas_buffer->restore_env();
    }

    // notice not pass canvas
    draw(*canvas_buffer);
    // we can use this buffer
    set_redraw_flag(false);
}

// use the buffer to copy to target
void Drawable::using_buffer_draw(Canvas &canvas) {
    bool flag = get_redraw_flag();
    if (!flag) {
        // the buffer is valid
        DBG(<< "using buffer texture...");
        canvas_buffer->paint_on_canvas(canvas);
    } else {
        DBG(<< "save buffer texture...");
        save_buffer(canvas);
        if (get_always_redraw()) {
            // althought we set always redraw, some elements must have a buffer to draw so that it can show itself in the target canvas
            canvas_buffer->paint_on_canvas(canvas);
            return;
        }
        // recursive
        using_buffer_draw(canvas);
    }
}

void Drawable::draw_border(Canvas &canvas, Element_status statu) {
    uint8_t r, g, b, a;
    // get_color(r, g, b, a, statu);
    get_border_color(r, g, b, a, statu);
    canvas.set_color(r, g, b, a);
    Rect rect = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    canvas.draw_shape(rect);
}

void Drawable::draw_background(Canvas &canvas, Element_status statu) {
    uint8_t r, g, b, a;
    get_background_color(r, g, b, a, statu);
    canvas.set_color(r, g, b, a);
    Rect rect = {0, 0, static_cast<double>(get_width()), static_cast<float>(get_height())};
    canvas.fill_shape(rect);
    Image *image = get_background_image(statu);
    if (image != nullptr) {
        image->draw_image(canvas, 0, 0);
    }
}
}
