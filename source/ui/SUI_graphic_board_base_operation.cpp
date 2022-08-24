#include "SUI_graphic_board_base.h"
#include "SUI_in_graphic_board_base_operation.h"

namespace sui {

GBBOP_move_to::GBBOP_move_to(int x, int y) : mx{x}, my{y} {}

void GBBOP_move_to::execute_operation(Graphic_board_base *board) {
    board->move_to(mx, my, true);
}

GBBOP_move::GBBOP_move(int dx, int dy) : mdx{dx}, mdy{dy} {}

void GBBOP_move::execute_operation(Graphic_board_base *board) {
    board->move(mdx, mdy, true);
}

GBBOP_line_to::GBBOP_line_to(int x, int y) : mx{x}, my{y} {}

void GBBOP_line_to::execute_operation(Graphic_board_base *board) {
    board->line_to(mx, my, true);
}

GBBOP_draw_line::GBBOP_draw_line(int x1, int y1, int x2, int y2) : mx1{x1}, my1{y1}, mx2{x2}, my2{y2} {}

void GBBOP_draw_line::execute_operation(Graphic_board_base *board) {
    board->draw_line(mx1, my1, mx2, my2, true);
}

GBBOP_fill_rect::GBBOP_fill_rect(const Rect &rect) : mrect{rect} {}

void GBBOP_fill_rect::execute_operation(Graphic_board_base *board) {
    board->fill_rect(mrect, true);
}

GBBOP_draw_rect::GBBOP_draw_rect(const Rect &rect) : mrect{rect} {}

void GBBOP_draw_rect::execute_operation(Graphic_board_base *board) {
    board->draw_rect(mrect, true);
}

GBBOP_draw_point::GBBOP_draw_point(const Point &point) : mpoint{point} {}

void GBBOP_draw_point::execute_operation(Graphic_board_base *board) {
    board->draw_point(mpoint, true);
}

GBBOP_draw_text::GBBOP_draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned font_size)
    : mrect{rect}, mstr{str}, mfont_name{font_name}, mcolor{color}, mfont_size{font_size} {}

void GBBOP_draw_text::execute_operation(Graphic_board_base *board) {
    board->draw_text(mrect, mstr, mfont_name, mcolor, mfont_size, true);
}

GBBOP_draw_image::GBBOP_draw_image(Image &img, int x, int y) : mimg{img}, mx{x}, my{y} {}

void GBBOP_draw_image::execute_operation(Graphic_board_base *board) {
    board->draw_image(mimg, mx, my, true);
}

void GBBOP_clear::execute_operation(Graphic_board_base *board) {
    board->clear(true);
}

GBBOP_set_draw_color::GBBOP_set_draw_color(const Color &color) : mcolor{color} {}

void GBBOP_set_draw_color::execute_operation(Graphic_board_base *board) {
    board->set_draw_color(mcolor, true);
}

GBBOP_fill_shape::GBBOP_fill_shape(const Shape &shape) : mshape{shape.clone()} {}

GBBOP_fill_shape::~GBBOP_fill_shape() {
    delete mshape;
}

void GBBOP_fill_shape::execute_operation(Graphic_board_base *board) {
    board->fill_shape(*mshape, true);
}

GBBOP_draw_shape::GBBOP_draw_shape(const Shape &shape) : mshape{shape.clone()} {}

GBBOP_draw_shape::~GBBOP_draw_shape() {
    delete mshape;
}

void GBBOP_draw_shape::execute_operation(Graphic_board_base *board) {
    board->fill_shape(*mshape, true);
}

}