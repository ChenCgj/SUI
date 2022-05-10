
#include "SUI_rect.h"
namespace sui {

Rect::Rect(int x1, int y1, int z1, int x2, int y2, int z2)
    : p1{x1, y1, z1}, p2{x2, y2, z2} {}

Rect::Rect(int x1, int y1, int x2, int y2) : Rect{x1, y1, 0, x2, y2, 0} {}

int Rect::get_width() const {
    return p2.x - p1.x;
}

int Rect::get_height() const {
    return p2.y - p1.y;
}
}