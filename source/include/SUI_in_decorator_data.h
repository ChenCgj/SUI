#ifndef SUI_IN_DECORATOR_DATA_H
#define SUI_IN_DECORATOR_DATA_H

#include "SDL_pixels.h"
#include "SUI_decorator.h"
namespace sui {
struct Decorator::Decorator_data {
    SDL_Color color;
    SDL_Color background_color;
};

}
#endif