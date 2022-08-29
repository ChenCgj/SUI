#include "SUI_pane_horizontal.h"

namespace sui {

Horizontal_pane::Horizontal_pane(int posX, int posY, int width, int height) :
    Geometry{posX, posY, width, height}, Pane_base{posX, posY, width, height}, gap{0} {
    
    object_name = "horizontal pane";
}

Horizontal_pane::~Horizontal_pane() {
    prepare_destroy();
}

void Horizontal_pane::set_gap(int gap) {
    this->gap = gap;
}

int Horizontal_pane::get_gap() const {
    return gap;
}

void Horizontal_pane::add_content(Geometry &node, int index) {
    int curr = 0;
    bool insert_ok = false;
    int left_temp = 0;
    for (auto iter = elements.begin(); iter != elements.end(); ++iter, ++curr) {
        if (curr == index) {
            iter = elements.insert(iter, &node);
            insert_ok = true;
            (*iter)->get_height_property().bind(get_height_property(), get_height_func());
            (*iter)->get_posY_property().bind(get_posY_property(), get_posY_func());
        }
        (*iter)->get_posX_property().unbind(get_posX_property());
        (*iter)->get_posX_property().bind(get_posX_property(), get_posX_func(left_temp));
        left_temp += gap + (*iter)->get_width();
    }
    if (!insert_ok) {
        elements.push_back(&node);
        elements.back()->get_posX_property().bind(get_posX_property(), get_posX_func(left_temp));
        elements.back()->get_height_property().bind(get_height_property(), get_height_func());
        elements.back()->get_posY_property().bind(get_posY_property(), get_posY_func());
    }
}

void Horizontal_pane::remove_content(Geometry &node) {
    node.get_posY_property().unbind(get_posY_property());
    node.get_height_property().unbind(get_height_property());
    node.get_posX_property().unbind(get_posX_property());
    elements.remove(&node);
    int left_temp = 0;
    for (auto pg : elements) {
        pg->get_posX_property().unbind(get_posX_property());
        pg->get_posX_property().bind(get_posX_property(), get_posX_func(left_temp));
        left_temp += pg->get_width() + gap;
    }
}

std::function<int (const int &)> Horizontal_pane::get_posX_func(int left_limit) {
    auto func = [=](const int &value) {
        return left_limit + gap + value;
    };
    return std::function<int (const int &)>(func);
}

std::function<int (const int &)> Horizontal_pane::get_posY_func() {
    auto func = [=](const int &value) {return value;};
    return std::function<int (const int &)>(func);
}

std::function<int (const int &)> Horizontal_pane::get_height_func() {
    auto func = [=](const int &value) {return value;};
    return std::function<int (const int &)>(func);
}
}