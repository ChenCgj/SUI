#ifndef SUI_IMAGE_H
#define SUI_IMAGE_H

#include "SUI_geometry.h"
#include "SUI_graphic_board.h"
#include <string>

namespace sui {

class Canvas;
class Sketch;
// class Graphic_board;

class Image : public virtual Geometry {
public:
    Image(int width, int height);
    ~Image();
    void load_img(const std::string &file);
    // void load_img(const Image &image);
    // void load_img(Sketch &sketch);
    void load_mask(const Graphic_board_base &board);
    void unload_mask();
    void draw_image(Canvas &canvas, int posX = 0, int posY = 0);
    unsigned get_image_width();
    unsigned get_image_height();
private:
    void update_image(Canvas &target_canvas);
    Canvas *pcanvas;
    Sketch *sketch;
    Graphic_board_base mask;
};
}
#endif