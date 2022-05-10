#include <functional>
#include "SUI_drawable.h"
#include "SUI_canvas.h"
#include "SUI_geometry.h"
#include "SUI_window.h"
#include "SUI_in_binder.h"
#include "SUI_property.h"
#include "SUI_in_debug.h"

namespace sui {
extern template class Property<int>;
extern template void Property<int>::add_binded<>(Property<int> &property, std::function<int (const int &)> func);

Drawable::Drawable(int posX, int posY, int posZ, int width, int height, int depth)
    : Geometry{posX, posY, posZ, width, height, depth},
    canvas_buffer{posX, posY, posZ, width, height, depth}, redraw_flag(true) {
    
    // to detect the size change, the canvas size should bind to the drawable element
    auto orign_value_func = std::function<int (const int &)>([](const int &v){return v;});
    canvas_buffer.get_width_property().bind(get_width_property(), orign_value_func);
    canvas_buffer.get_height_property().bind(get_height_property(), orign_value_func);
    canvas_buffer.get_posX_property().bind(get_posX_property(), orign_value_func);
    canvas_buffer.get_posY_property().bind(get_posY_property(), orign_value_func);
}

Drawable::Drawable(int width, int height)
    : Drawable(0, 0, 0, width, height, 0) {}

Drawable::Drawable(int posX, int posY, int width, int height)
    : Drawable(posX, posY, 0, width, height, 0) {}

Drawable::~Drawable() = default;

void Drawable::draw_all(Canvas &canvas) {
    draw(canvas);
}

void Drawable::set_redraw_flag(bool flag) {
    redraw_flag = flag;
}

bool Drawable::get_redraw_flag() {
    return redraw_flag;
}

void Drawable::save_buffer(Canvas &canvas) {
    // load the render information
    canvas_buffer.load_renderer(canvas);
    // clean the buffer
    canvas_buffer.save_env();
    canvas_buffer.set_color(0, 0, 0, 0);
    canvas_buffer.clear();
    canvas_buffer.restore_env();

    // notice not pass canvas
    draw(canvas_buffer);
    // we can use this buffer
    set_redraw_flag(false);
}

// use the buffer to copy to target
void Drawable::using_buffer_draw(Canvas &canvas) {
    bool flag = get_redraw_flag();
    if (!flag) {
        // the buffer is valid
        canvas_buffer.paint_on_canvas(canvas);
    } else {
        DBG(<< "save buffer texture...");
        save_buffer(canvas);
        // recursive
        using_buffer_draw(canvas);
    }
}
}
