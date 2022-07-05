#include <string>
#include "SDL_surface.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "SUI_geometry.h"
#include "SUI_in_image.h"
#include "SUI_in_debug.h"

namespace sui {

Image::Image(int posX, int posY, int width, int height)
    : Geometry(posX, posY, width, height), image_surface{nullptr}, image_texture{nullptr}, source_area{0, 0, 0, 0} {}

void Image::destroy_image() {
    if (!image_texture) {
        SDL_DestroyTexture(image_texture);
    }
    if (!image_surface) {
        SDL_FreeSurface(image_surface);
    }
    image_surface = nullptr;
    image_texture = nullptr;
}

unsigned Image::get_image_width() const {
    if (image_surface == nullptr) {
        return 0;
    } else {
        return image_surface->w;
    }
}

unsigned Image::get_image_height() const {
    if (image_surface == nullptr) {
        return 0;
    } else {
        return image_surface->h;
    }
}

void Image::set_source_area(int x, int y, int w, int h) {
    source_area.x = x;
    source_area.y = y;
    source_area.w = w;
    source_area.h = h;
}

bool Image::load_image(const std::string &image_file) {
    if (image_file == "") {
        destroy_image();
        return true;
    }
    std::string::size_type pos = image_file.find_last_of('.');
    std::string suffix = image_file.substr(pos);
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
    if (image_surface) {
        destroy_image();
    }
    image_surface = IMG_Load(image_file.c_str());
    if (image_surface == nullptr) {
        ERR(<< "IMG load err: " << IMG_GetError());
        return false;
    }
    set_source_area(0, 0, get_image_width(), get_image_height());
    IMG_Quit();
    return true;
}

void Image::draw_image(Canvas &canvas) {
    canvas.draw_image(*this);
}

Image::~Image() {
    destroy_image();
}
}