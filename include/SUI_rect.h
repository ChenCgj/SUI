#ifndef SUI_RECT_H
#define SUI_RECT_H
#include "SUI_point.h"
namespace sui {
class Rect {
public:
    Rect(int x1, int y1, int z1, int x2, int y2, int z2);
    Rect(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
    int get_width() const;
    int get_height() const;
    Point p1, p2;
};
}

#endif