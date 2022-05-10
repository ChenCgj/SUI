#include "SUI_element.h"
#include "SUI_canvas.h"
#include "SUI_drawable.h"
#include "SUI_in_main.h"
#include "SUI_in_debug.h"
namespace sui {

Element::Element(int posX, int posY, int width, int height) : Drawable(posX, posY, width, height) {
    object_name = "Element";
    // we should add the sub object to the trash root until it was add to a parent
    if (!TRASH_ROOT->add_node(this)) {
        ERR(<< "Element added to root failure.");
    }
}

// simply draw a rect contain the element
void Element::draw(Canvas &canvas) {
    DBG(<< "draw element...");
    canvas.save_env();
    canvas.set_color(255, 255, 255, 255);
    canvas.draw_rect(Rect{0, 0, get_width(), get_height()});
    canvas.restore_env();
}

Element::~Element() = default;
}