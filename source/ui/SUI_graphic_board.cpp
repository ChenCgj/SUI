#include <functional>
#include <stdlib.h>

#include "SUI_drawable.h"
#include "SUI_geometry.h"
#include "SUI_graphic_board.h"
#include "SDL_render.h"
#include "SUI_graphic_board_base.h"
#include "SUI_in_canvas.h"
#include "SUI_element.h"
#include "SUI_main.h"
#include "SUI_in_debug.h"

namespace sui {

Graphic_board::Graphic_board(int posX, int posY, int width, int height)
    : Geometry(posX, posY, width, height), Graphic_board_base{width, height}, Element(posX, posY, width, height) {
    object_name = "graphic_board";
    // arg = this;
    // delete_arg = false;
    statu = Element_status::normal;
}

Graphic_board::~Graphic_board() = default;

void Graphic_board::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw graphic board start...");
    canvas.save_env();
    draw_background(canvas, statu);
    draw_border(canvas, statu);
    draw_board(canvas);
    canvas.restore_env();

    DBG(<< get_name() << "draw graphic board ok");
}

void Graphic_board::destroy_content() {
    unload_data();
    Element::destroy_content();
}

}