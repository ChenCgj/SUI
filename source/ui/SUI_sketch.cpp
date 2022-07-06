#include <string>
#include "SDL_surface.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "SUI_geometry.h"
#include "SUI_in_sketch.h"
#include "SUI_in_debug.h"

namespace sui {

Sketch::Sketch(int posX, int posY, int width, int height)
    : Geometry(posX, posY, width, height), sketch_surface{nullptr}, sketch_texture{nullptr}, source_area{0, 0, 0, 0} {}

void Sketch::destroy_sketch() {
    if (!sketch_texture) {
        SDL_DestroyTexture(sketch_texture);
    }
    if (!sketch_surface) {
        SDL_FreeSurface(sketch_surface);
    }
    sketch_surface = nullptr;
    sketch_texture = nullptr;
}

unsigned Sketch::get_sketch_width() const {
    if (sketch_surface == nullptr) {
        return 0;
    } else {
        return sketch_surface->w;
    }
}

unsigned Sketch::get_sketch_height() const {
    if (sketch_surface == nullptr) {
        return 0;
    } else {
        return sketch_surface->h;
    }
}

void Sketch::set_source_area(int x, int y, int w, int h) {
    source_area.x = x;
    source_area.y = y;
    source_area.w = w;
    source_area.h = h;
}

bool Sketch::load_sketch(const std::string &sketch_file) {
    if (sketch_file == "") {
        destroy_sketch();
        return true;
    }
    std::string::size_type pos = sketch_file.find_last_of('.');
    std::string suffix = sketch_file.substr(pos);
    int rtn = -1;
    if (suffix == ".PNG" || suffix == ".png") {
        rtn = IMG_Init(IMG_INIT_PNG);
    } else if (suffix == ".JPG" || suffix == ".jpg") {
        rtn = IMG_Init(IMG_INIT_JPG);
    } else if (suffix == ".WEBP" || suffix == ".tif") {
        rtn = IMG_Init(IMG_INIT_WEBP);
    } else {
        return false;
    }
    if (rtn < 0) {
        return false;
    }
    if (sketch_surface) {
        destroy_sketch();
    }
    sketch_surface = IMG_Load(sketch_file.c_str());
    if (sketch_surface == nullptr) {
        ERR(<< "IMG load err: " << IMG_GetError());
        return false;
    }
    set_source_area(0, 0, get_sketch_width(), get_sketch_height());
    IMG_Quit();
    return true;
}

void Sketch::draw_sketch(Canvas &canvas) {
    canvas.draw_sketch(*this);
}

Sketch::~Sketch() {
    destroy_sketch();
}
}