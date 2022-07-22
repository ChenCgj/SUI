#ifndef SUI_PANE_H
#define SUI_PANE_H
#include "SUI_pane_base.h"

namespace sui {
class Pane : public Pane_base {
public:
    Pane(int posX, int posY, int width, int height);
    void add_content(Geometry &element, int x, int y);
    void remove_content(Geometry &element);
private:
    std::function<int (const int &)> get_posX_func(int x);
    std::function<int (const int &)> get_posY_func(int y);
};
}
#endif