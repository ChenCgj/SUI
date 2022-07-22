#include "SUI_pane.h"

namespace sui {

Pane::Pane(int posX, int posY, int width, int height) : Geometry{posX, posY, width, height},
    Pane_base{posX, posY, width, height} {
    
    object_name = "pane";
}

void Pane::add_content(Geometry &element, int x, int y) {
    element.get_posX_property().bind(get_posX_property(), get_posX_func(x));
    element.get_posY_property().bind(get_posY_property(), get_posY_func(y));
}

void Pane::remove_content(Geometry &element) {
    element.get_posX_property().unbind(get_posX_property());
    element.get_posY_property().unbind(get_posY_property());
}

std::function<int (const int &)> Pane::get_posX_func(int x) {
    auto func = [=](const int value) {return value + x;};
    return std::function<int (const int &)>(func);
}

std::function<int (const int &)> Pane::get_posY_func(int y) {
    auto func = [=](const int value) {return value + y;};
    return std::function<int (const int &)>(func);
}

}