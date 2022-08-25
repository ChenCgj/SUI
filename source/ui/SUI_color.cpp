#include "SUI_color.h"

namespace sui {
bool operator==(const Color &c1, const Color &c2) {
    return c1.red == c2.red &&
           c1.green == c2.green &&
           c1.blue == c2.blue &&
           c1.alpha == c2.alpha;
}

bool operator!=(const Color &c1, const Color &c2) {
    return !(c1 == c2);
}
}