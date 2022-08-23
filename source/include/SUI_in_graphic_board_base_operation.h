#ifndef SUI_IN_GRAPHIC_BOARD_BASE_OPERATION_H
#define SUI_IN_GRAPHIC_BOARD_BASE_OPERATION_H

#include <string>

#include "SUI_shape.h"
#include "SUI_color.h"

namespace sui {
class Image;
class Graphic_board_base;

class GBB_operation {
public:
    virtual void execute_operation(Graphic_board_base *board) = 0;
    virtual ~GBB_operation() {};
};

class GBBOP_move_to : public GBB_operation {
public:
    GBBOP_move_to(int x, int y);
    void execute_operation(Graphic_board_base *board) override;
private:
    int mx, my;
};

class GBBOP_move : public GBB_operation {
public:
    GBBOP_move(int dx, int dy);
    void execute_operation(Graphic_board_base *board) override;
private:
    int mdx, mdy;
};

class GBBOP_line_to : public GBB_operation {
public:
    GBBOP_line_to(int x, int y);
    void execute_operation(Graphic_board_base *board) override;
private:
    int mx, my;
};

class GBBOP_draw_line : public GBB_operation {
public:
    GBBOP_draw_line(int x1, int y1, int x2, int y2);
    void execute_operation(Graphic_board_base *board) override;
private:
    int mx1, my1, mx2, my2;
};

class GBBOP_fill_rect : public GBB_operation {
public:
    GBBOP_fill_rect(const Rect &rect);
    void execute_operation(Graphic_board_base *board) override;
private:
    Rect mrect;
};

class GBBOP_draw_rect : public GBB_operation {
public:
    GBBOP_draw_rect(const Rect &rect);
    void execute_operation(Graphic_board_base *board) override;
private:
    Rect mrect;
};

class GBBOP_draw_point : public GBB_operation {
public:
    GBBOP_draw_point(const Point &point);
    void execute_operation(Graphic_board_base *board) override;
private:
    Point mpoint;
};

class GBBOP_draw_text : public GBB_operation {
public:
    GBBOP_draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned font_size);
    void execute_operation(Graphic_board_base *board) override;
private:
    Rect mrect;
    std::string mstr, mfont_name;
    Color mcolor;
    unsigned mfont_size;
};

class GBBOP_draw_image : public GBB_operation {
public:
    GBBOP_draw_image(Image &img, int x, int y);
    void execute_operation(Graphic_board_base *board) override;
private:
    Image &mimg;
    int mx, my;
};

class GBBOP_clear : public GBB_operation {
public:
    void execute_operation(Graphic_board_base *board) override;
};

class GBBOP_set_color : public GBB_operation {
public:
    GBBOP_set_color(const Color &color);
    void execute_operation(Graphic_board_base *board) override;
private:
    Color mcolor;
};

/**
* @todo make the shape copy construct ok
*/

class GBBOP_fill_shape : public GBB_operation {
public:
    GBBOP_fill_shape(const Shape &shape);
    void execute_operation(Graphic_board_base *board) override;
    ~GBBOP_fill_shape();
private:
    Shape *mshape;
};

class GBBOP_draw_shape : public GBB_operation {
public:
    GBBOP_draw_shape(const Shape &shape);
    void execute_operation(Graphic_board_base *board) override;
    ~GBBOP_draw_shape();
private:
    Shape *mshape;
};
}
#endif