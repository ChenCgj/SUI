#include "SUI_in_debug.h"
#include "SUI_in_canvas.h"

#include "SUI_label.h"

namespace sui {

Label::Label(const std::string &text, unsigned font_size, int x, int y, int w, int h) : Geometry{x, y, w, h}, Element{x, y, w, h}, text{text}, font_size{font_size} {
    object_name = "label";
}

void Label::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw label start...");
    canvas.save_env();
    draw_background(canvas);
    draw_border(canvas);
    Color color = {0, 0, 0, 255};
    get_color(color.red, color.green, color.blue, color.alpha);
    Rect r = {0, 0, static_cast<double>(get_width()), static_cast<double>(get_height())};
    canvas.draw_text(r, text, "consola.ttf", color, font_size);
    canvas.restore_env();
    DBG(<< get_name() << "draw label ok");
}

Label::~Label() {
    DBG(<< get_name() << "was destroy.");
}

void Label::set_text(const std::string &text) {
    this->text = text;
}

const std::string &Label::get_text() {
    return text;
}
}