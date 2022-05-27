#include <cstddef>
#include <memory>
#include <string.h>

#include "SUI_decorator.h"
#include "SUI_in_decorator_data.h"
#include "SUI_in_main.h"
#include "SUI_in_styles.h"
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