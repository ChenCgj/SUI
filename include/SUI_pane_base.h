#ifndef SUI_PANE_BASE_H
#define SUI_PANE_BASE_H

#include "SUI_element.h"

namespace sui {

class Pane_base : public Element {
public:
    Pane_base(int posX, int posY, int width, int height);
    ~Pane_base();
};

}
#endif