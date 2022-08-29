#ifndef FONT_H
#define FONT_H

#include "pixel_block.h"

class Font {
public:
    static Font *instance();
    void init_font();
    void print_num(Pixel_screen *screen, int posX, int posY, int num);
    void print_heart(Pixel_screen *screen, int posX, int posY);
    void print_big_bullet(Pixel_screen *screen, int posX, int posY, int index);
private:
    Pixel_block mnums[10];
    Pixel_block heart;
    Pixel_block big_bullet[3];
};
#endif