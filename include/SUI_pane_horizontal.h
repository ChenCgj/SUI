#ifndef SUI_PANE_HORIZONTAL_H
#define SUI_PANE_HORIZONTAL_H
#include "SUI_pane_base.h"

namespace sui {

class Horizontal_pane : public Pane_base {
public:
    Horizontal_pane(int posX, int posY, int width, int height);
    void add_content(Geometry &node, int index = -1);
    void remove_content(Geometry &node);
    void set_gap(int gap);
    int get_gap() const;
    ~Horizontal_pane();
private:
    int gap;
    std::list<Geometry *> elements;
    std::function<int (const int &)> get_posX_func(int left_limit);
    std::function<int (const int &)> get_posY_func();
    std::function<int (const int &)> get_height_func();
};
}
#endif