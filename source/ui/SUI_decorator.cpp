#include <cstddef>
#include <memory>

#include "SUI_decorator.h"
#include "SUI_in_decorator_data.h"

namespace sui {
Decorator::Decorator() : pData(nullptr) {
    pData = std::make_unique<Decorator_data>();
}

void Decorator::set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    pData->color.r = red;
    pData->color.g = green;
    pData->color.b = blue;
    pData->color.a = alpha;
}

void Decorator::get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const {
    red = pData->color.r;
    green = pData->color.g;
    blue = pData->color.b;
    alpha = pData->color.a;
}

void Decorator::set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    pData->background_color.r = red;
    pData->background_color.g = green;
    pData->background_color.b = blue;
    pData->background_color.a = alpha;
}

void Decorator::get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const {
    red = pData->background_color.r;
    green = pData->background_color.g;
    blue = pData->background_color.b;
    alpha = pData->background_color.a;
}

Decorator::~Decorator() = default;
}