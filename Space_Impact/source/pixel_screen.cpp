#include "pixel_screen.h"

Pixel_screen::Pixel_screen(unsigned int width, unsigned int height, sui::Color init_color)
    : Pixel_element(width, height, init_color), show_board{nullptr} {}

Pixel_screen::Pixel_screen(unsigned int width, unsigned int height, Pixel_color init_color)
    : Pixel_element(width, height, init_color), show_board(nullptr) {};

void Pixel_screen::update() {
    if (show_board) {
        show_board->set_need_redraw(true);
    }
}

void Pixel_screen::set_pixel(int posX, int posY, Pixel_color color) {
    Pixel_element::set_pixel(posX, posY, color);
    update();
}

void Pixel_screen::set_pixel(int posX, int posY, const sui::Color &color) {
    Pixel_element::set_pixel(posX, posY, color);
    update();
}

void Pixel_screen::show_on_board(sui::Graphic_board_base *board) {
    if (!board) {
        return;
    }
    show_board = board;
    board->set_draw_callback([&](sui::Graphic_board_base *arg)->void {
        int width = arg->get_width();
        int height = arg->get_height();
        unsigned pwidth = get_width(), pheight = get_height();
        double pixel_w = width * 1.0 / pwidth;
        double pixel_h = height * 1.0 / pheight;
        sui::Rect r = {0, 0, pixel_w, pixel_h};
        sui::Color curr_color = {0, 0, 0, 255};
        arg->set_draw_color(curr_color, true);
        for (int i = 0; i < pwidth; ++i) {
            for (int j = 0; j < pheight; ++j) {
                if (*get_pixel(i, j) != curr_color) {
                    curr_color = *get_pixel(i, j);
                    arg->set_draw_color(curr_color, true);
                }
                arg->fill_rect(r, true);
                r.p1.y += pixel_h;
                r.p2.y += pixel_h;
            }
            r.p1.y = 0;
            r.p2.y = pixel_h;
            r.p1.x += pixel_w;
            r.p2.x += pixel_w;
        }
        arg->set_need_redraw(true);
        sui::present_all();
    });
}

