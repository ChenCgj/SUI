#ifndef SUI_GRAPHIC_BOARD_BASE_H
#define SUI_GRAPHIC_BOARD_BASE_H

#include <functional>
#include <string>
#include <vector>

#include "SUI_color.h"
#include "SUI_geometry.h"
#include "SUI_shape.h"

namespace sui {
class Image;
class GBB_operation;

class Graphic_board_base : public virtual Geometry {
public:
    Graphic_board_base(int width, int height);
    void set_draw_callback(const std::function<void (Graphic_board_base *)> draw_func/*, void *func_arg, bool delete_arg*/);
    std::function<void (Graphic_board_base *)> get_draw_callback() const;
    void draw_board(Canvas &canvas, int posX = 0, int posY = 0);
    void move_to(int x, int y, bool callback_flag = false);
    void move(int dx, int dy, bool callback_flag = false);
    void line_to(int x, int y, bool callback_flag = false);
    void draw_line(int x1, int y1, int x2, int y2, bool callback_flag = false);
    void fill_rect(const Rect &rect, bool callback_flag = false);
    void draw_rect(const Rect &rect, bool callback_flag = false);
    void clear(bool callback_flag = false);
    void set_color(const Color &color, bool callback_flag = false);
    void fill_shape(const Shape &shape, bool callback_flag = false);
    void draw_shape(const Shape &shape, bool callback_flag = false);
    void draw_point(const Point &point, bool callback_flag = false);
    void draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned font_size, bool callback_flag = false);
    void draw_image(Image &img, int x = 0, int y = 0, bool callback_flag = false);
    // to destroy texture before destroy the renderer
    void unload_data();
    void set_need_redraw(bool flag);
    void clear_draw_operation();
    virtual ~Graphic_board_base();
private:
    std::function<void (Graphic_board_base *)> draw_callback;
    // std::function<void (void *arg)> free_arg;
    // void *arg;
    // bool delete_arg;
    Canvas *board;
    std::vector<GBB_operation *> draw_operations;
    Point curr_pos;
    friend class Event_handler_helper;
};
}
#endif