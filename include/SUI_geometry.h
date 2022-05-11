/**
* @file SUI_geometry.h
* @brief contains the definiton of Geometry class
*/

#ifndef SUI_GEOMETRY_H
#define SUI_GEOMETRY_H

#include "SUI_property.h"
namespace sui {

/**
* @class Geometry
* @brief provide the size, position information, which class need to show in window should derived from this class
*/

class Geometry {
public:
    Geometry(int posX, int posY, int width, int height);
    Geometry(int posX, int posY, int poxZ, int widht, int height, int depth);
    virtual int get_posX();
    virtual int get_posY();
    virtual int get_posZ();
    virtual int get_width();
    virtual int get_height();
    virtual int get_depth();
    virtual void set_posX(int x);
    virtual void set_posY(int y);
    virtual void set_width(int w);
    virtual void set_height(int h);
    virtual void set_posZ(int z);
    virtual void set_depth(int d);
    Property<int> &get_posX_property();
    Property<int> &get_posY_property();
    Property<int> &get_width_property();
    Property<int> &get_height_property();
    Property<int> &get_posZ_property();
    Property<int> &get_depth_property();
    virtual ~Geometry();
private:
    Property<int> posX;
    Property<int> posY;
    Property<int> posZ;
    Property<int> width;
    Property<int> height;
    Property<int> depth;
};

}
#endif