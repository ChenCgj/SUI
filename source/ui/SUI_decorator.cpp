#include <cstddef>
#include <memory>
#include <string.h>

#include "SUI_decorator.h"
#include "SUI_geometry.h"
#include "SUI_in_decorator_data.h"
#include "SUI_in_main.h"
#include "SUI_in_styles.h"
#include "SUI_in_theme.h"
#include "SUI_main.h"
#include "SUI_in_theme_manager.h"
#include "SUI_in_debug.h"

namespace sui {
Decorator::Decorator() : pData(nullptr) {
    pData = std::make_unique<Decorator_data>();
    pData->theme = nullptr;
    pData->index = current_theme;
}

Decorator::~Decorator() {
    if (pData->index == none_theme) {
        delete pData->theme;
    }
}

void Decorator::set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Element_status statu) {
    set_theme(none_theme);
    pData->theme->set_color(statu, Theme::Style_option::color, Color{red, green, blue, alpha});
}

void Decorator::get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha, Element_status statu) const {
    Color color;
    if (pData->index != none_theme) {
        color = THEME_MANAGER->get_theme(pData->index).get_color(statu, Theme::Style_option::color);
    } else {
        color = pData->theme->get_color(statu, Theme::Style_option::color);
    }
    red = color.red;
    green = color.green;
    blue = color.blue;
    alpha = color.alpha;
}

void Decorator::set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Element_status statu) {
    set_theme(none_theme);
    pData->theme->set_background_color(statu, Theme::Style_option::background_color, Color{red, green, blue, alpha});
}

void Decorator::get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha, Element_status statu) const {
    Color color;
    if (pData->index != none_theme) {
        color = THEME_MANAGER->get_theme(pData->index).get_background_color(statu, Theme::Style_option::background_color);
    } else {
        color = pData->theme->get_background_color(statu, Theme::Style_option::background_color);
    }
    red = color.red;
    green = color.green;
    blue = color.blue;
    alpha = color.alpha;
}

void Decorator::set_background_image(const std::string &image_file, const Rect &target_rect, Element_status statu) {
    set_theme(none_theme);
    pData->theme->set_background_image(statu, Theme::Style_option::background_image, image_file, target_rect);
}

Image *Decorator::get_background_image(Element_status statu) {
    Image *image = nullptr;
    if (pData->index != none_theme) {
        image = THEME_MANAGER->get_theme(pData->index).get_background_image(statu, Theme::Style_option::background_image);
    } else {
        image = pData->theme->get_background_image(statu, Theme::Style_option::background_image);
    }
    return image;
}

void Decorator::set_background_fill_style(Background_fill_style fill_style, void *data/* Geometry *geometry OR Rect *target_size */, Element_status statu) {
    Image *image = get_background_image(statu);
    if (image == nullptr) {
        return;
    }
    set_theme(none_theme);
    pData->theme->set_background_fill_style(statu, Theme::Style_option::background_fill_style, fill_style);
    if (fill_style == Background_fill_style::full) {
        Geometry *geometry = reinterpret_cast<Geometry *>(data);
        image->get_width_property().bind(geometry->get_width_property(), std::function<int (const int &)>([](const int &x)->int {return x;}));
        image->get_height_property().bind(geometry->get_height_property(), std::function<int (const int &)>([](const int &x)->int {return x;}));
        image->set_posX(0);
        image->set_posY(0);
    } else if (fill_style == Background_fill_style::orign_size) {
        /**
        * @todo unbind all
        */
        image->set_posX(0);
        image->set_posY(0);
        image->set_width(image->get_image_width());
        image->set_height(image->get_image_height());
    } else if (fill_style == Background_fill_style::target_size) {
        /**
        * @todo unbind all
        */
        Rect *target_size = reinterpret_cast<Rect *>(data);
        image->set_posX(target_size->p1.x);
        image->set_posY(target_size->p1.y);
        image->set_width(target_size->get_width());
        image->set_height(target_size->get_height());
    }
}

int Decorator::get_border_radius(Element_status statu) const {
    if (pData->index != none_theme) {
        return THEME_MANAGER->get_theme(pData->index).get_border_radius(statu, Theme::Style_option::border_radius);
    }
    return pData->theme->get_border_radius(statu, Theme::Style_option::border_radius);
}

void Decorator::set_border_radius(unsigned int radius, Element_status statu) {
    set_theme(none_theme);
    pData->theme->set_border_radius(statu, Theme::Style_option::border_radius, radius);
}

void Decorator::set_theme(Theme_index index) {
    if (index == pData->index) {
        return;
    }
    if (pData->index == none_theme) {
        delete pData->theme;
    }
    pData->index = index;
    if (pData->index == none_theme) {
        pData->theme = new Theme{};
        if (!pData->theme) {
            ERR(<< "theme is nullptr");
        }
    }
}
}