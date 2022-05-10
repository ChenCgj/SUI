#include "SUI_geometry.h"
#include "SUI_property.h"
#include "SUI_window.h"

namespace sui {
extern template class Property<int>;

Geometry::Geometry(int posX, int posY, int width, int height)
    : Geometry{posX, posY, 0, width, height, 0} {}

Geometry::Geometry(int posX, int posY, int posZ, int width, int height, int depth)
    : posX{posX}, posY{posY}, posZ(posZ), width{width}, height{height}, depth{depth} {}

Geometry::~Geometry() = default;

int Geometry::get_posX() {
    return posX.get_value();
}

void Geometry::set_posX(int x) {
    posX.set_value(x);
}

int Geometry::get_posY() {
    return posY.get_value();
}

void Geometry::set_posY(int y) {
    posY.set_value(y);
}

void Geometry::set_posZ(int z) {
    posZ.set_value(z);
}

int Geometry::get_posZ() {
    return posZ.get_value();
}

int Geometry::get_width() {
    return width.get_value();
}

void Geometry::set_width(int w) {
    width.set_value(w);
}

int Geometry::get_height() {
    return height.get_value();
}

void Geometry::set_height(int h) {
    height.set_value(h);
}

void Geometry::set_depth(int d) {
    depth.set_value(d);
}

int Geometry::get_depth() {
    return depth.get_value();
}

Property<int> &Geometry::get_posX_property() {
    return posX;
}

Property<int> &Geometry::get_posY_property() {
    return posY;
}

Property<int> &Geometry::get_width_property() {
    return width;
}

Property<int> &Geometry::get_height_property() {
    return height;
}

Property<int> &Geometry::get_depth_property() {
    return depth;
}

}