#ifndef SUI_IN_STYLES_H
#define SUI_IN_STYLES_H

#include "SUI_styles.h"
#include "SUI_color.h"

namespace sui {
class Image;

struct Border_style {
    Color border_left_color;
    Color border_top_color;
    Color border_right_color;
    Color border_bottom_color;
    int border_radius;
};

/**
* @todo make the background image can fill the background
*/
struct Background_style {
    Background_fill_style fill_style;
    Color background_color;
    Image *background_image;
};

struct Style {
    Color color;
    Border_style border;
    Background_style background;
};

struct Button_style {
    Style normal;
    Style hover;
    Style pressed;
};
}
#endif