#include <cstddef>
#include <memory>
#include <string.h>

#include "SUI_decorator.h"
#include "SUI_in_decorator_data.h"
#include "SUI_in_styles.h"

namespace sui {
Decorator::Decorator() : pData(nullptr) {
    pData = std::make_unique<Decorator_data>();
    memset(&pData->style, 0, sizeof(Style));
    pData->style.color.alpha = 255;
}

Decorator::~Decorator() = default;

void Decorator::set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    pData->style.color.red = red;
    pData->style.color.green = green;
    pData->style.color.blue = blue;
    pData->style.color.alpha = alpha;
}

void Decorator::get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const {
    red = pData->style.color.red;
    green = pData->style.color.green;
    blue = pData->style.color.blue;
    alpha = pData->style.color.alpha;
}

void Decorator::set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    pData->style.background.background_color.red = red;
    pData->style.background.background_color.green = green;
    pData->style.background.background_color.blue = blue;
    pData->style.background.background_color.alpha = alpha;
}

void Decorator::get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const {
    red = pData->style.background.background_color.red;
    green = pData->style.background.background_color.green;
    blue = pData->style.background.background_color.blue;
    alpha = pData->style.background.background_color.alpha;
}

int Decorator::get_border_radius() const {
    return pData->style.border.border_radius;
}

void Decorator::set_border_radius(unsigned int radius) {
    pData->style.border.border_radius = radius;
}
}