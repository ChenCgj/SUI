#include "pixel_element.h"

Pixel_element::Pixel_element(unsigned int width, unsigned int height, const sui::Color &color)
    : mwidth{width}, mheight{height}, mpixels{mheight, std::vector<sui::Color>(mwidth, color)} {}

Pixel_element::Pixel_element() : Pixel_element(0, 0, black) {}

Pixel_element::Pixel_element(unsigned int width, unsigned int height, Pixel_color color)
    : Pixel_element{width, height, sui::Color{0, 0, 0, 0}} {
    sui::Color c;
    get_color(color, c);
    for (int i = 0; i < mheight; ++i) {
        for (int j = 0; j < mwidth; ++j) {
            mpixels[i][j] = c;
        }
    }
}

Pixel_element::~Pixel_element() {}

void Pixel_element::set_pixel(int posX, int posY, const sui::Color &color) {
    if (color.alpha == 0 || posX < 0 || posY < 0 || posX >= mwidth || posY >= mheight) {
        return;
    }
    mpixels[posY][posX] = color;
}

void Pixel_element::set_pixel(int posX, int posY, Pixel_color color) {
    sui::Color c;
    get_color(color, c);
    set_pixel(posX, posY, c);
}

void Pixel_element::clean(const sui::Color &color) {
    for (int i = 0; i < mheight; ++i) {
        for (int j = 0; j < mwidth; ++j) {
            mpixels[i][j] = color;
        }
    }
}

const sui::Color *Pixel_element::get_pixel(int posX, int posY) const {
    if (posX < 0 || posY < 0 || posX >= mwidth || posY >= mheight) {
        return nullptr;
    }
    return &mpixels[posY][posX];
}

void Pixel_element::draw_on_other(Pixel_element *element, int target_posX, int target_posY) const {
    for (int y = 0; y < mheight && target_posY + y < int(element->mheight); ++y) {
        for (int x = 0; x < mwidth && target_posX + x < int(element->mwidth); ++x) {
            const sui::Color *pcolor = get_pixel(x, y);
            if (pcolor != nullptr) {
                element->set_pixel(target_posX + x, target_posY + y, *pcolor);
            }
        }
    }
}

int Pixel_element::get_width() const {
    return mwidth;
}

int Pixel_element::get_height() const {
    return mheight;
}

void Pixel_element::get_color(Pixel_color pcolor, sui::Color &color) {
    switch (pcolor) {
    case white:
        color = {255, 255, 255, 255};
        break;
    case grey:
        color = {128, 128, 128, 255};
        break;
    case black:
        color = {0, 0, 0, 255};
        break;
    default:
        color = {0, 0, 0, 255};
        break;
    }
}