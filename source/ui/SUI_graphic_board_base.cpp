#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_graphic_board_base.h"
#include "SUI_image.h"

namespace sui {

Graphic_board_base::Graphic_board_base(int width, int height)
    : Geometry{0, 0, width, height}, draw_callback{nullptr}, board{new Canvas(0, 0, 0, width, height, 0)} {
    auto func = std::function<int (const int &)>([](const int &x){return x;});
    board->get_width_property().bind(get_width_property(), func);
    board->get_height_property().bind(get_height_property(), func);
    // arg = this;
    // delete_arg = false;
}

Graphic_board_base::~Graphic_board_base() {
    delete board;
}

void Graphic_board_base::unload_data() {
    board->unload_renderer();
}

void Graphic_board_base::set_draw_callback(const std::function<void (Graphic_board_base *)> draw_func/*, void *func_arg, bool del_arg*/) {
    draw_callback = draw_func;
    // arg = func_arg;
    // delete_arg = del_arg;
    // if (arg == nullptr || arg == this) {
    //     arg = this;
    //     delete_arg = false;
    // }
}

std::function<void (Graphic_board_base *)> Graphic_board_base::get_draw_callback() const {
    return draw_callback;
}

void Graphic_board_base::draw_board(Canvas &canvas, int posX, int posY) {
    if (!board) {
        ERR(<< "invalid board.");
        return;
    }
    board->load_renderer(canvas);
    if (board->check_need_redraw() || !board->isValid()) {
        board->save_env();
        if (draw_callback) {
            draw_callback(this);
        }
        board->present();
        board->restore_env();
        board->set_need_redraw(false);
    }
    board->set_posX(posX);
    board->set_posY(posY);
    board->paint_on_canvas(canvas);
}

void Graphic_board_base::move_to(int x, int y) {
    curr_pos.x = x;
    curr_pos.y = y;
}

void Graphic_board_base::move(int dx, int dy) {
    curr_pos.x += dx;
    curr_pos.y += dy;
}

void Graphic_board_base::line_to(int x, int y) {
    board->draw_line(curr_pos, Point(x, y, 0));
    curr_pos.x = x;
    curr_pos.y = y;
}

void Graphic_board_base::draw_line(int x1, int y1, int x2, int y2) {
    curr_pos.x = x2;
    curr_pos.y = y2;
    board->draw_line(Point(x1, y1, 0), curr_pos);
}

void Graphic_board_base::fill_rect(const Rect &rect) {
    board->fill_rect(rect);
}

void Graphic_board_base::draw_rect(const Rect &rect) {
    board->draw_rect(rect);
}

void Graphic_board_base::draw_point(const Point &point) {
    board->draw_point(point);
}

void Graphic_board_base::draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned int font_size) {
    board->draw_text(rect, str, font_name, color, font_size);
}

void Graphic_board_base::draw_image(Image &img, int x, int y) {
    img.draw_image(*board, x, y);
}

void Graphic_board_base::clear() {
    board->clear();
}

void Graphic_board_base::set_color(const Color &color) {
    board->set_color(color.red, color.green, color.blue, color.alpha);
}

void Graphic_board_base::fill_shape(const Shape &shape) {
    board->fill_shape(shape);
}

void Graphic_board_base::draw_shape(const Shape &shape) {
    board->draw_shape(shape);
}

}