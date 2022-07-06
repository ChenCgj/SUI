#include "SUI_graphic_board.h"
#include "SUI_canvas.h"
#include "SUI_element.h"
#include "SUI_main.h"
#include "SUI_in_debug.h"

namespace sui {

Graphic_board::Graphic_board(int posX, int posY, int width, int height) : Element(posX, posY, width, height) {
    object_name = "graphic_board";
    draw_callback = nullptr;
    // arg = this;
    // delete_arg = false;
    statu = Element_status::normal;
    pcanvas = nullptr;
}

void Graphic_board::set_draw_callback(const std::function<void ()> draw_func/*, void *func_arg, bool del_arg*/) {
    draw_callback = draw_func;
    // arg = func_arg;
    // delete_arg = del_arg;
    // if (arg == nullptr || arg == this) {
    //     arg = this;
    //     delete_arg = false;
    // }
}

void Graphic_board::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw graphic board start...");
    Canvas *temp = pcanvas;
    pcanvas = &canvas;
    pcanvas->save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    if (draw_callback) {
        draw_callback();
    }
    pcanvas->restore_env();
    pcanvas = temp;
    DBG(<< get_name() << "draw graphic board ok");
}

// Graphic_board::~Graphic_board() = default;

void Graphic_board::move_to(int x, int y) {
    curr_pos.x = x;
    curr_pos.y = y;
}

void Graphic_board::move(int dx, int dy) {
    curr_pos.x += dx;
    curr_pos.y += dy;
}

void Graphic_board::line_to(int x, int y) {
    pcanvas->draw_line(curr_pos, Point(x, y, 0));
    curr_pos.x = x;
    curr_pos.y = y;
}

void Graphic_board::draw_line(int x1, int y1, int x2, int y2) {
    curr_pos.x = x2;
    curr_pos.y = y2;
    pcanvas->draw_line(Point(x1, y1, 0), curr_pos);
}

void Graphic_board::fill() {
    Rect rect(0, 0, get_width(), get_height());
    pcanvas->fill_rect(rect);
}

void Graphic_board::set_color(const Color &color) {
    pcanvas->set_color(color.red, color.green, color.blue, color.alpha);
}

void Graphic_board::start_draw_on_image() {
    pcanvas->save_env();
}

void Graphic_board::end_draw_on_image() {
    pcanvas->restore_env();
}
}