#include "SUI_pane_grid.h"

namespace sui {

Grid_pane::Grid_pane(int posX, int posY, int width, int height, int row, int col) : Geometry{posX, posY, width, height},
    Pane_base{posX, posY, width, height}, row_count{row}, col_count{col}, row_gap{0}, col_gap{0} {
    
    object_name = "grid pane";
}

Grid_pane::~Grid_pane() {
    prepare_destroy();
}

void Grid_pane::set_grid(int row, int col) {
    row_count = row;
    col_count = col;
}

int Grid_pane::get_row_count() const {
    return row_count;
}

int Grid_pane::get_col_count() const {
    return col_count;
}

void Grid_pane::set_gap(int row_gap, int col_gap) {
    this->row_gap = row_gap;
    this->col_gap = col_gap;
}

int Grid_pane::get_row_gap() const {
    return row_gap;
}

int Grid_pane::get_col_gap() const {
    return col_gap;
}

void Grid_pane::add_content(Geometry &node, int row, int col) {
    node.get_posX_property().bind(get_width_property(), get_posX_func(row, col));
    node.get_posY_property().bind(get_height_property(), get_posY_func(row, col));
    node.get_width_property().bind(get_width_property(), get_width_func(row, col));
    node.get_height_property().bind(get_height_property(), get_height_func(row, col));
}

void Grid_pane::remove_content(Geometry &node) {
    node.get_posX_property().unbind(get_width_property());
    node.get_posY_property().unbind(get_height_property());
    node.get_width_property().unbind(get_width_property());
    node.get_height_property().unbind(get_height_property());
}

std::function<int (const int &)> Grid_pane::get_posX_func(int row, int col) {
    auto func = [=](const int &value) {
        double per_col = (value - col_gap * (col_count + 1)) * 1.0 / col_count;
        return static_cast<int>(per_col * col + col_gap * (col + 1) + get_posX());
    };
    // return static_cast<std::function<int (int)>>(func);
    return func;
}

std::function<int (const int &)> Grid_pane::get_posY_func(int row, int col) {
    auto func = [=](const int &value) {
        double per_row = (value - row_gap * (row_count + 1)) * 1.0 / row_count;
        return static_cast<int>(per_row * row + row_gap * (row + 1) + get_posY());
    };
    // return static_cast<std::function<int (int)>>(func);
    return func;
}

std::function<int (const int &)> Grid_pane::get_width_func(int row, int col) {
    auto func = [=](const int &value) {
        double per_col = (value - col_gap * (col_count + 1)) * 1.0 / col_count;
        return per_col;
    };
    // return static_cast<std::function<int (int)>>(func);
    return func;
}

std::function<int (const int &)> Grid_pane::get_height_func(int row, int col) {
    auto func = [=](const int &value) {
        double per_row = (value - row_gap * (row_count + 1)) * 1.0 / row_count;
        return per_row;
    };
    // return static_cast<std::function<int (int)>>(func);
    return func;
}
}