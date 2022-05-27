#ifndef SUI_IN_DECORATOR_DATA_H
#define SUI_IN_DECORATOR_DATA_H

#include "SUI_decorator.h"
#include "SUI_in_styles.h"
#include "SUI_in_theme.h"
#include "SUI_main.h"

namespace sui {
struct Decorator::Decorator_data {
    Theme_index index;
    Theme *theme;
};

}
#endif