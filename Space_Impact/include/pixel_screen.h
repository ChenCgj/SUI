#ifndef PIXEL_SCREEN_H
#define PIXEL_SCREEN_H

#include <vector>

#include "pixel_element.h"

class Pixel_screen : public Pixel_element {
public:
    Pixel_screen(unsigned int width, unsigned int height, Pixel_color init_color = black);
    Pixel_screen(unsigned int width, unsigned int height, sui::Color init_color);
    void show_on_board(sui::Graphic_board_base * board);
    void set_pixel(int posX, int posY, Pixel_color color) override;
    void set_pixel(int posX, int posY, const sui::Color &color) override;
    void update();
private:
    sui::Graphic_board_base *show_board;
};
#endif