#ifndef SUI_IN_THEME_H
#define SUI_IN_THEME_H
#include <string>
#include "SDL_render.h"
#include "SUI_in_canvas.h"
#include "SUI_in_main.h"
#include "SUI_in_styles.h"
#include "SUI_styles.h"
#include "SUI_main.h"

namespace sui {

class Theme {
public:
    enum Style_option {
        border_left_color,
        border_right_color,
        border_top_color,
        border_bottom_color,
        border_radius,
        color,
        background_color,
        background_image,
        background_fill_style
    };
    Color get_border_color(Element_status statu, Style_option edge);
    bool set_border_color(Element_status statu, Style_option edge, const Color &color);
    Color get_color(Element_status statu, Style_option bg_fg);
    bool set_color(Element_status statu, Style_option bg_fg, const Color &color);
    Color get_background_color(Element_status statu, Style_option bg_color);
    bool set_background_color(Element_status statu, Style_option bg_color, const Color &color);
    bool set_background_image(Element_status statu, Style_option bg_image, const std::string &image_file, const Rect &target_rect, const Rect &src_area = {0, 0, 0, 0});
    Image *get_background_image(Element_status statu, Style_option bg_image);
    bool set_background_fill_style(Element_status statu, Style_option bg_image, Background_fill_style fill_style);
    int get_border_radius(Element_status statu, Style_option edge);
    bool set_border_radius(Element_status statu, Style_option edge, int radius);
    Theme();
    ~Theme();
private:
    Style *get_style(Element_status statu);
    Button_style button_style;
    Style normal_style;
};

}
#endif