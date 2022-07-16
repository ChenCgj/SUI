#ifndef SUI_IN_DECORATOR_DATA_H
#define SUI_IN_DECORATOR_DATA_H

#include "SUI_decorator.h"

namespace sui {
class Theme;
struct Decorator::Decorator_data {
    Theme_index index;
    Theme *theme;
};

}
#endif