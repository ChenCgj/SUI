#ifndef SUI_PANE_VERTICAL_H
#define SUI_PANE_VERTICAL_H
#include "SUI_pane_base.h"

namespace sui {

class Vertical_pane : public Pane_base {
public:
    Vertical_pane(int posX, int posY, int width, int height);
    void add_content(Geometry &node, int index = -1);
    void remove_content(Geometry &node);
    void set_gap(int gap);
    int get_gap() const;
    ~Vertical_pane();
private:
    int gap;
    std::list<Geometry *> elements;
    std::function<int (const int &)> get_posY_func(int top_limit);
    std::function<int (const int &)> get_posX_func();
    std::function<int (const int &)> get_width_func();
};
}
#endif