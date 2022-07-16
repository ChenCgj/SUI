#include "SUI_image.h"
#include "SUI_in_canvas.h"
#include "SUI_in_sketch.h"

namespace sui {

Image::Image(int width, int height) : Geometry{0, 0, width, height}, pcanvas{new Canvas(0, 0, 0, width, height, 0)},
    sketch{new Sketch{width, height}}, mask{width, height} {
    auto func = std::function<int (const int &)>([](const int &x)->int {return x;});
    pcanvas->get_width_property().bind(get_width_property(), func);
    pcanvas->get_height_property().bind(get_height_property(), func);
    mask.get_width_property().bind(get_width_property(), func);
    mask.get_height_property().bind(get_height_property(), func);
    // may be sketch doesn't need bind
    sketch->get_width_property().bind(get_width_property(), func);
    sketch->get_height_property().bind(get_height_property(), func);
    mask.set_draw_callback([](Graphic_board_base *arg){
        arg->set_color(Color{255, 255, 255, 255});
        arg->clear();
    });
}

Image::~Image() {
    if (sketch) {
        delete sketch;
    }
    if (pcanvas) {
        delete pcanvas;
    }
}

void Image::load_img(const std::string &file, const Rect &src_area) {
    sketch->load_sketch(file);
    sketch->set_source_area(src_area.p1.x, src_area.p1.y, src_area.get_width(), src_area.get_height());
}

void Image::unload_data() {
    pcanvas->unload_renderer();
    sketch->unload_data();
    mask.unload_data();
}

void Image::load_mask(const Graphic_board_base &board) {
    mask.set_draw_callback(board.get_draw_callback());
}

void Image::unload_mask() {
    mask.set_draw_callback([&](Graphic_board_base *arg){
        arg->set_color(Color{255, 255, 255, 255});
        arg->clear();
    });
}

void Image::update_image(Canvas &target_canvas) {
    pcanvas->load_renderer(target_canvas);
    sketch->draw_sketch(*pcanvas, 0, 0);
    pcanvas->set_mask_mode(Mask_mode::be_masked);
    mask.draw_board(*pcanvas, 0, 0);
    pcanvas->set_mask_mode(Mask_mode::none_mask);
}

void Image::draw_image(Canvas &canvas, int posX, int posY) {
    if (!pcanvas->isValid() || pcanvas->check_need_redraw()) {
        update_image(canvas);
        pcanvas->set_need_redraw(false);
    }
    pcanvas->set_posX(posX);
    pcanvas->set_posY(posY);
    pcanvas->paint_on_canvas(canvas);
}

unsigned Image::get_image_width() {
    if (sketch) {
        return sketch->get_sketch_width();
    } else {
        return 0;
    }
}

unsigned Image::get_image_height() {
    if (sketch) {
        return sketch->get_sketch_height();
    } else {
        return 0;
    }
}

}