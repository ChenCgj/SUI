#ifndef SUI_GRAPHIC_BOARD_BASE_H
#define SUI_GRAPHIC_BOARD_BASE_H

#include <functional>
#include "SUI_color.h"
#include "SUI_geometry.h"
#include "SUI_shape.h"

namespace sui {

class Canvas;

class Graphic_board_base : public virtual Geometry {
public:
    Graphic_board_base(int width, int height);
    void set_draw_callback(const std::function<void (Graphic_board_base *)> draw_func/*, void *func_arg, bool delete_arg*/);
    std::function<void (Graphic_board_base *)> get_draw_callback() const;
    void draw_board(Canvas &canvas, int posX = 0, int posY = 0);
    void move_to(int x, int y);
    void move(int dx, int dy);
    void line_to(int x, int y);
    void draw_line(int x1, int y1, int x2, int y2);
    void fill_rect(const Rect &rect);
    void clear();
    void set_color(const Color &color);
    void fill_shape(const Shape &shape);
    virtual ~Graphic_board_base();
private:
    std::function<void (Graphic_board_base *)> draw_callback;
    // std::function<void (void *arg)> free_arg;
    // void *arg;
    // bool delete_arg;
    Canvas *board;
    Point curr_pos;
    friend class Event_handler_helper;
};
}
#endif