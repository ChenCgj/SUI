#include <utility>
#include "SUI_in_canvas.h"
#include "SUI_in_events.h"
#include "SUI_in_debug.h"
#include "SUI_tool.h"
#include "SUI_slider.h"

using std::pair;
using std::make_pair;

namespace sui {
Slider::Slider(int x, int y, int w, int h, double min, double max) : Geometry{x, y, w, h}, Element{x, y, w, h},
    m_min{min}, m_max{max}, m_index{0}, cb_slide{nullptr}, a_slide{nullptr} {

    object_name = "slider";
    statu = Element_status::normal;
}

Slider::~Slider() {
    DBG(<< get_name() << "was destroy.");
    prepare_destroy();
}

// return the rect of the block relative to the slider
Rect Slider::get_block_rect() const {
    double bw = k_block_width, bh = std::max(get_height() - 4, 0);
    double bx = m_index;
    double by = 2;
    return Rect{bx, by, bx + bw, by + bh};
}

void Slider::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw slider start...");
    canvas.save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    draw_slide_block(canvas);
    canvas.restore_env();
    DBG(<< get_name() << "draw slider ok");
}

void Slider::draw_slide_block(Canvas &canvas) {
    
    uint8_t r, g, b, a;
    get_color(r, g, b, a);
    canvas.set_color(r, g, b, a);
    canvas.fill_shape(get_block_rect());
}

void Slider::add_listener(const std::function<void (const Mouse_motion_event &, void *)> &func, Slider_event event, void *arg) {
    switch (event) {
    case Slider_event::se_slide:
        a_slide = arg;
        cb_slide = func;
        break;
    default:
        ERR(<< "Unkndow type for Mouse_motion_event");
    }
}

void Slider::set_value(double value) {
    double p = get_width() - k_block_width;
    if (p <= 0 || m_min >= m_max) {
        m_index = 0;
        return;
    }
    value = std::max(m_min, std::min(value, m_max));
    m_index = (value - m_min) / (m_max - m_min) * p;
}

double Slider::get_value() const {
    double p = get_width() - k_block_width;
    if (p <= 0) {
        return 0;
    }
    return m_index / p * (m_max - m_min) + m_min;
}

void Slider::set_range(double min, double max) {
    m_min = min;
    m_max = max;
}

pair<int, int> Slider::get_range() const {
    return make_pair(m_min, m_max);
}

void Slider::deal_mouse_move_event(Mouse_motion_event &move_event) {
    Element::deal_mouse_move_event(move_event);
    if (move_event.handle()) {
        return;
    }
    double mouse_x = move_event.event.button.x;
    double mouse_y = move_event.event.button.y;
    auto r = get_rect();
    if (r.is_point_in(Point{mouse_x, mouse_y})) {
        if (!move_event.left_button_down()) {
            return;
        }
        m_index += move_event.get_relative_pos().first;
        m_index = std::max(0.0, std::min(m_index, double(get_width() - k_block_width)));
        if (cb_slide) {
            cb_slide(move_event, a_slide);
        }
        set_redraw_flag(true);
        present_all();
    }
}
}