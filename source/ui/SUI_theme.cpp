#include "SDL_render.h"
#include "SUI_color.h"
#include "SUI_in_main.h"
#include "SUI_in_styles.h"
#include "SUI_in_theme.h"
#include "SUI_in_debug.h"
#include "SUI_main.h"

namespace sui {

void clean_button_background_image(Button_style &style);

Theme::Theme() {
    normal_style.border.border_bottom_color =
    normal_style.border.border_left_color =
    normal_style.border.border_right_color =
    normal_style.border.border_top_color =
    normal_style.color = Color{0, 0, 0, 255};

    normal_style.background.background_color = Color{255, 255, 255, 255};
    normal_style.background.background_image = nullptr;
    normal_style.background.fill_style = Background_fill_style::target_size;

    button_style.normal = button_style.hover = button_style.pressed = normal_style;
    button_style.normal.background.background_color = Color{0xd5, 0xd5, 0xd5, 0xff};
    button_style.normal.background.background_image = nullptr;
    button_style.normal.background.fill_style = Background_fill_style::target_size;
    button_style.pressed.background.background_color = Color{0x3f, 0x3f, 0x3f, 0xff};
    button_style.pressed.background.background_image = nullptr;
    button_style.pressed.background.fill_style = Background_fill_style::target_size;
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

Sketch *Theme::get_background_image(Element_status statu, Style_option background_image) {
    Sketch *image = nullptr;
    Style *style = get_style(statu);
    if (style) {image = style->background.background_image;}
    return image;
}

bool Theme::set_background_image(Element_status statu, Style_option bg_image, const std::string &image_file, const Rect &target_rect) {
    Style *style = get_style(statu);
    if (!style) {
        ERR(<< "the style is nullptr");
        return false;
    }
    if (bg_image != Style_option::background_image) {
        return false;
    }
    if (!style->background.background_image) {
        style->background.background_image = new Sketch(0, 0, 0, 0);
    }
    style->background.background_image->load_sketch(image_file);
    style->background.background_image->set_width(target_rect.get_width());
    style->background.background_image->set_height(target_rect.get_height());
    style->background.background_image->set_posX(target_rect.p1.x);
    style->background.background_image->set_posY(target_rect.p1.y);
    return true;
}

bool Theme::set_background_fill_style(Element_status statu, Style_option bg_image, Background_fill_style fill_style) {
    Style *style = get_style(statu);
    if (!style) {
        ERR(<< "the style is nullptr");
        return false;
    }
    if (bg_image != Style_option::background_image) {
        return false;
    }
    style->background.fill_style = fill_style;
    return true;
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

Theme::~Theme() {
    if (normal_style.background.background_image) {
        delete normal_style.background.background_image;
        normal_style.background.background_image = nullptr;
    }
    clean_button_background_image(button_style);
}

void clean_button_background_image(Button_style &style) {
    if (style.normal.background.background_image) {
        delete style.normal.background.background_image;
    }
    if (style.hover.background.background_image) {
        delete style.hover.background.background_image;
    }
    if (style.pressed.background.background_image) {
        delete style.pressed.background.background_image;
    }
    style.normal.background.background_image = style.hover.background.background_image = style.pressed.background.background_image = nullptr;
}
}