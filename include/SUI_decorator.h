/**
* @file SUI_decorator.h
* @brief contain definition of class Decorator
*/

#ifndef SUI_DECORATOR_H
#define SUI_DECORATOR_H

#include <memory>
#include <stdint.h>
#include <string>

#include "SUI_main.h"
#include "SUI_geometry.h"
#include "SUI_styles.h"
#include "SUI_main.h"
#include "SUI_shape.h"
#include "SUI_image.h"

namespace sui {
/**
* @class Decorator
* @brief decorate some element which derived from this class
*/

class Decorator {
public:
    Decorator();
    virtual ~Decorator();
    void set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Element_status statu = Element_status::normal);
    void get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha, Element_status statu = Element_status::normal) const;
    void set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Element_status statu = Element_status::normal);
    void get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha, Element_status statu = Element_status::normal) const;
    void set_background_image(const std::string &image_file, const Rect &target_rect, const Rect &src_area = {0, 0, 0, 0}, Element_status statu = Element_status::normal);
    Image *get_background_image(Element_status statu = Element_status::normal);
    void set_background_fill_style(Background_fill_style fill_style, void *data, Element_status statu = Element_status::normal);
    void set_border_radius(unsigned radius, Element_status statu = Element_status::normal);
    int get_border_radius(Element_status statu = Element_status::normal) const;
    void set_theme(Theme_index index);
private:
    void check_data();
    struct Decorator_data;
    std::unique_ptr<Decorator_data> pData;
};
}
#endif