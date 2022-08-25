#ifndef SUI_COLOR_H
#define SUI_COLOR_H

#include <stdint.h>
namespace sui {
class Color {
public:
    uint8_t red, green, blue, alpha;
    friend bool operator==(const Color &c1, const Color &c2);
    friend bool operator!=(const Color &c1, const Color &c2);
};

bool operator==(const Color &c1, const Color &c2);
bool operator!=(const Color &c1, const Color &c2);

}
#endif