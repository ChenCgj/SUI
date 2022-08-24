#include "SUI_in_canvas.h"
#include "SUI_in_debug.h"
#include "SUI_in_graphic_board_base_operation.h"
#include "SUI_graphic_board_base.h"
#include "SUI_image.h"

namespace sui {

Graphic_board_base::Graphic_board_base(int width, int height)
    : Geometry{0, 0, width, height}, draw_callback{nullptr}, board{new Canvas(0, 0, 0, width, height, 0)}, draw_operations{} {
    auto func = std::function<int (const int &)>([](const int &x){return x;});
    board->get_width_property().bind(get_width_property(), func);
    board->get_height_property().bind(get_height_property(), func);
    // arg = this;
    // delete_arg = false;
}

Graphic_board_base::~Graphic_board_base() {
    clear_draw_operation();
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
        for (auto op : draw_operations) {
            op->execute_operation(this);
        }
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

void Graphic_board_base::set_need_redraw(bool flag) {
    board->set_need_redraw(flag);
}

void Graphic_board_base::move_to(int x, int y, bool callback_flag) {
    if (callback_flag) {
        curr_pos.x = x;
        curr_pos.y = y;
    } else {
        GBB_operation *op = new GBBOP_move_to{x, y};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::move(int dx, int dy, bool callback_flag) {
    if (callback_flag) {
        curr_pos.x += dx;
        curr_pos.y += dy;
    } else {
        GBB_operation *op = new GBBOP_move{dx, dy};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::line_to(int x, int y, bool callback_flag) {
    if (callback_flag) {
        board->draw_line(curr_pos, Point(x, y, 0));
        curr_pos.x = x;
        curr_pos.y = y;
    } else {
        GBB_operation *op = new GBBOP_line_to{x, y};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_line(int x1, int y1, int x2, int y2, bool callback_flag) {
    if (callback_flag) {
        curr_pos.x = x2;
        curr_pos.y = y2;
        board->draw_line(Point(x1, y1, 0), curr_pos);
    } else {
        GBB_operation *op = new GBBOP_draw_line{x1, y1, x2, y2};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::fill_rect(const Rect &rect, bool callback_flag) {
    if (callback_flag) {
        board->fill_rect(rect);
    } else {
        GBB_operation *op = new GBBOP_fill_rect{rect};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_rect(const Rect &rect, bool callback_flag) {
    if (callback_flag) {
        board->draw_rect(rect);
    } else {
        GBB_operation *op = new GBBOP_draw_rect{rect};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_point(const Point &point, bool callback_flag) {
    if (callback_flag) {
        board->draw_point(point);
    } else {
        GBB_operation *op = new GBBOP_draw_point{point};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned int font_size, bool callback_flag) {
    if (callback_flag) {
        board->draw_text(rect, str, font_name, color, font_size);
    } else {
        GBB_operation *op = new GBBOP_draw_text{rect, str, font_name, color, font_size};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_image(Image &img, int x, int y, bool callback_flag) {
    if (callback_flag) {
        img.draw_image(*board, x, y);
    } else {
        GBB_operation *op = new GBBOP_draw_image{img, x, y};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::clear(bool callback_flag) {
    if (callback_flag) {
        board->clear();
    } else {
        GBB_operation *op = new GBBOP_clear{};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::set_draw_color(const Color &color, bool callback_flag) {
    if (callback_flag) {
        board->set_color(color.red, color.green, color.blue, color.alpha);
    } else {
        GBB_operation *op = new GBBOP_set_draw_color{color};
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::fill_shape(const Shape &shape, bool callback_flag) {
    if (callback_flag) {
        board->fill_shape(shape);
    } else {
        GBB_operation *op = new GBBOP_fill_shape(shape);
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::draw_shape(const Shape &shape, bool callback_flag) {
    if (callback_flag) {
        board->draw_shape(shape);
    } else {
        GBB_operation *op = new GBBOP_draw_shape(shape);
        draw_operations.push_back(op);
    }
}

void Graphic_board_base::clear_draw_operation() {
    for (const auto op : draw_operations) {
        delete op;
    }
    draw_operations.clear();
}

}