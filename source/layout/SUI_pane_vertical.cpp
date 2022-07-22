#include "SUI_pane_vertical.h"

namespace sui {

Vertical_pane::Vertical_pane(int posX, int posY, int width, int height) :
    Geometry{posX, posY, width, height}, Pane_base{posX, posY, width, height}, gap{0} {

    object_name = "vertical pane";
}

void Vertical_pane::set_gap(int gap) {
    this->gap = gap;
}

int Vertical_pane::get_gap() const {
    return gap;
}

void Vertical_pane::add_content(Geometry &node, int index) {
    int curr = 0;
    bool insert_ok = false;
    int height_temp = 0;
    for (auto iter = elements.begin(); iter != elements.end(); ++iter, ++curr) {
        if (curr == index) {
            iter = elements.insert(iter, &node);
            insert_ok = true;
            (*iter)->get_width_property().bind(get_width_property(), get_width_func());
            (*iter)->get_posX_property().bind(get_posX_property(), get_posX_func());
        }
        (*iter)->get_posY_property().unbind(get_posY_property());
        (*iter)->get_posY_property().bind(get_posY_property(), get_posY_func(height_temp));
        height_temp += gap + (*iter)->get_height();
    }
    if (!insert_ok) {
        elements.push_back(&node);
        elements.back()->get_posY_property().bind(get_posY_property(), get_posY_func(height_temp));
        elements.back()->get_width_property().bind(get_width_property(), get_width_func());
        elements.back()->get_posX_property().bind(get_posX_property(), get_posX_func());
    }
}

void Vertical_pane::remove_content(Geometry &node) {
    node.get_posY_property().unbind(get_posY_property());
    node.get_width_property().unbind(get_width_property());
    node.get_posX_property().unbind(get_posX_property());
    elements.remove(&node);
    int height_temp = 0;
    for (auto pg : elements) {
        pg->get_posY_property().unbind(get_posY_property());
        pg->get_posY_property().bind(get_posY_property(), get_posY_func(height_temp));
        height_temp += pg->get_height() + gap;
    }
}

std::function<int (const int &)> Vertical_pane::get_posY_func(int top_limit) {
    auto func = [=](const int &value) {
        return top_limit + gap + value;
    };
    return std::function<int (const int &)>(func);
}

std::function<int (const int &)> Vertical_pane::get_posX_func() {
    auto func = [=](const int &value) {return value;};
    return std::function<int (const int &)>(func);
}

std::function<int (const int &)> Vertical_pane::get_width_func() {
    auto func = [=](const int &value) {return value;};
    return std::function<int (const int &)>(func);
}
}