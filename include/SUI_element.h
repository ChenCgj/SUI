/**
* @file SUI_element.h
* @brief contains definition of class Element
*/

#ifndef SUI_ELEMENT_H
#define SUI_ELEMENT_H

#include "SUI_drawable.h"
#include "SUI_main.h"
#include "SUI_object.h"
namespace sui {

/**
* @class Element
* @brief usually a ui element should be derived from this class
*/
class Element : public Object, public Drawable {
public:
    Element(int posX, int posY, int width, int height);
    virtual void draw(Canvas &canvas) override;
    virtual ~Element();
protected:
    Element_status statu;
};

}
#endif