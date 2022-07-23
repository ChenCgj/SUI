#include "SDL_image.h"

#include "SUI_in_canvas.h"
#include "SUI_in_sketch.h"
#include "SUI_in_debug.h"
#include "SUI_in_managers.h"
#include "SUI_in_texture_sdl_manager.h"

namespace sui {

Sketch::Sketch(int width, int height)
    : Geometry(0, 0, width, height), sketch_surface{nullptr},
    image_texture_id{TEXTURE_SDL_MANAGER->alloc_texture_id()},
    texture_id{TEXTURE_SDL_MANAGER->alloc_texture_id()},
    source_area{0, 0, 0, 0} {}

void Sketch::unload_data() {
    TEXTURE_SDL_MANAGER->set_texture(image_texture_id, nullptr, nullptr);
    TEXTURE_SDL_MANAGER->set_texture(texture_id, nullptr, nullptr);
}

void Sketch::destroy_sketch() {
    unload_data();
    if (!sketch_surface) {
        SDL_FreeSurface(sketch_surface);
    }
    sketch_surface = nullptr;
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
    int rtn = 1;
    if (suffix == ".PNG" || suffix == ".png") {
        // rtn = IMG_Init(IMG_INIT_PNG);
    } else if (suffix == ".JPG" || suffix == ".jpg") {
        // rtn = IMG_Init(IMG_INIT_JPG);
    } else if (suffix == ".WEBP" || suffix == ".webp") {
        // rtn = IMG_Init(IMG_INIT_WEBP);
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
    // IMG_Quit();
    return true;
}

void Sketch::draw_sketch(Canvas &canvas, int posX, int posY) {
    set_posX(posX);
    set_posY(posY);
    canvas.save_env();
    canvas.draw_sketch(*this);
    canvas.restore_env();
}

Sketch::~Sketch() {
    // may have problem when create static object
    destroy_sketch();
    TEXTURE_SDL_MANAGER->free_texture_id(image_texture_id);
    TEXTURE_SDL_MANAGER->free_texture_id(texture_id);
}

}