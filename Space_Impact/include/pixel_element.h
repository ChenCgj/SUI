#ifndef PIXEL_ELEMENT_H
#define PIXEL_ELEMENT_H

#include "SUI.h"

class Pixel_element {
public:
    enum Pixel_color {
        black, grey, white
    };
    Pixel_element();
    Pixel_element(unsigned int width, unsigned int height, Pixel_color init_color = black);
    Pixel_element(unsigned int width, unsigned int height, const sui::Color &init_color);
    virtual void set_pixel(int posX, int posY, Pixel_color color);
    virtual void set_pixel(int posX, int posY, const sui::Color &color);
    void clean(const sui::Color &color);
    const sui::Color *get_pixel(int posX, int posY) const;
    void draw_on_other(Pixel_element *element, int target_posX, int target_posY) const;
    static void get_color(Pixel_color pcolor, sui::Color &color);
    int get_width() const;
    int get_height() const;
    virtual ~Pixel_element();
protected:
    unsigned int mwidth, mheight;
    std::vector<std::vector<sui::Color>> mpixels;
};

#endif