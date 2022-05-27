#include "SDL_render.h"
#include "SUI_color.h"
#include "SUI_in_main.h"
#include "SUI_in_styles.h"
#include "SUI_in_theme.h"
#include "SUI_in_debug.h"

namespace sui {

Theme::Theme() {
    normal_style.border.border_bottom_color =
    normal_style.border.border_left_color =
    normal_style.border.border_right_color =
    normal_style.border.border_top_color =
    normal_style.color = Color{0, 0, 0, 255};

    normal_style.background.background_color = Color{255, 255, 255, 255};
    normal_style.background.background_image = nullptr;

    button_style.normal = button_style.hover = button_style.pressed = normal_style;
    button_style.normal.background.background_color = Color{0xd5, 0xd5, 0xd5, 0xff};
    button_style.pressed.background.background_color = Color{0x3f, 0x3f, 0x3f, 0xff};
    button_style.normal.color = Color{0, 0, 0, 255};
}

Color Theme::get_border_color(Element_status statu, Style_option edge) {
    Color color = {0, 0, 0, 255};
    Style *style = get_style(statu);
    if (!style) {return color;}

    switch (edge) {
    case border_left_color:
        return style->border.border_left_color;
        break;
    case border_right_color:
        return style->border.border_right_color;
        break;
    case border_bottom_color:
        return style->border.border_bottom_color;
        break;
    case border_top_color:
        return style->border.border_top_color;
        break;
    default:
        ERR(<< "use invalid Style_option");
        return color;
    }
};

bool Theme::set_border_color(Element_status statu, Style_option edge, const Color &color) {
    Style *style = get_style(statu);
    if (!style) {return false;}

    switch (edge) {
    case border_left_color:
        style->border.border_left_color = color;
        break;
    case border_right_color:
        style->border.border_right_color = color;
        break;
    case border_bottom_color:
        style->border.border_bottom_color = color;
        break;
    case border_top_color:
        style->border.border_top_color = color;
        break;
    default:
        ERR(<< "use invalid Style_option");
        return false;
    }
    return true;
}

int Theme::get_border_radius(Element_status statu, Style_option edge) {
    Style *style = get_style(statu);
    if (!style) {
        ERR(<< "the style is nullptr");
        return 0;
    }
    return style->border.border_radius;
}

bool Theme::set_border_radius(Element_status statu, Style_option edge, int radius) {
    Style *style = get_style(statu);
    if (!style) {
        ERR(<< "the style is nullptr");
        return false;
    }
    style->border.border_radius = radius;
    return true;
}

Color Theme::get_background_color(Element_status statu, Style_option bg_color) {
    Color color{0, 0, 0, 255};
    Style *style = get_style(statu);
    if (style) {return style->background.background_color;}
    else {ERR(<< "the style is nullptr"); return color;}
}

bool Theme::set_background_color(Element_status statu, Style_option bg_colorm, const Color &color) {
    Style *style = get_style(statu);
    if (!style) {ERR(<< "the style is nullptr"); return false;}
    style->background.background_color = color;
    return true;
}

SDL_Texture *Theme::get_background_image(Element_status statu, Style_option background_image) {
    SDL_Texture *image = nullptr;
    Style *style = get_style(statu);
    if (style) {image = style->background.background_image;}
    return image;
}

bool Theme::set_color(Element_status statu, Style_option bg_fg, const Color &color) {
    Style *style = get_style(statu);
    if (!style) {ERR(<< "the style is nullptr"); return false;}
    style->color = color;
    return true;
}

Color Theme::get_color(Element_status statu, Style_option bg_fg) {
    Color color{0, 0, 0, 255};
    Style *style = get_style(statu);
    if (!style) {
        ERR(<< "the style is nullptr");
        return color;
    }

    switch (bg_fg) {
    case Style_option::color:
        return style->color;
    default:
        ERR(<< "invalid Style option");
        break;
    }
    return color;
}

Style *Theme::get_style(Element_status statu) {
    Style *pstyle = nullptr;
    switch (statu) {
    case None:
        return nullptr;
        break;
    case normal:
        pstyle = &normal_style;
        break;
    case button_normal:
        pstyle = &button_style.normal;
        break;
    case button_press:
        pstyle = &button_style.pressed;
        break;
    case button_hover:
        pstyle = &button_style.hover;
        break;
    default:
        ERR(<< "invalid element statu");
        break;
    }
    return pstyle;
}
}