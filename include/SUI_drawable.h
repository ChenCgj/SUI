/**
* @file SUI_drawable.h
* @brief contains definiton of class Drawable
*/

#ifndef SUI_DRAWABLE_H
#define SUI_DRAWABLE_H

#include "SUI_decorator.h"
#include "SUI_geometry.h"
#include "SUI_main.h"
#include "SUI_shape.h"

namespace sui {
class Canvas;
/**
* @class Drawable
* @brief any element can be drawed on the window should be derived this class
*/
class Drawable : public Geometry, public Decorator {
public:
    Drawable(int width, int height);
    Drawable(int posX, int posY, int width, int height);
    Drawable(int posX, int posY, int posZ, int width, int height, int depth);
    // draw the element to the canvas
    virtual void draw(Canvas &canvas) = 0;
    // draw all elements it cantains to the canvas
    virtual void draw_all(Canvas &canvas);
    // call this function to prevent repeating draw
    void using_buffer_draw(Canvas &canvas);
    // set the element should be redraw to the buffer canvas or not
    void set_redraw_flag(bool flag);
    bool get_redraw_flag();
    void destroy_content();
    virtual ~Drawable();
protected:
    virtual void draw_border(Canvas &canvas, Element_status statu = Element_status::normal);
    virtual void draw_background(Canvas &canvas, Element_status statu = Element_status::normal);
private:
    // draw the element to buffer canvas, the param is aim to provide the render information
    void save_buffer(Canvas &canvas);
    // the buffer
    Canvas *canvas_buffer;
    bool redraw_flag;
};
}
#endif