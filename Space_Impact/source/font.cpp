#include "font.h"

void Font::init_font() {
    std::string resorce_name = "./resource/num0.dat";
    for (int i = 0; i < 10; ++i) {
        resorce_name[14] = '0' + i;
        mnums[i].load_data(resorce_name);
    }
    heart.load_data("./resource/heart.dat");
    for (int i = 0; i < 3; ++i) {
        big_bullet[i].load_data(std::string("./resource/bigbullet") + char(('1' + i)) + ".dat");
    }
}

Font *Font::instance() {
    static Font *pfont = nullptr;
    if (!pfont) {
        static Font font;
        font.init_font();
        pfont = &font;
    }
    return pfont;
}

void Font::print_num(Pixel_screen *screen, int posX, int posY, int num) {
    if (num < 0 || num > 9) {
        return;
    }
    mnums[num].draw_on_other(screen, posX, posY);
}

void Font::print_heart(Pixel_screen *screen, int posX, int posY) {
    heart.draw_on_other(screen, posX, posY);
}

void Font::print_big_bullet(Pixel_screen *screen, int posX, int posY, int index) {
    if (index < 0 || index > 3) {
        return;
    }
    big_bullet[index].draw_on_other(screen, posX, posY);
}