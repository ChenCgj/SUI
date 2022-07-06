/**
* @file SUI_in_image.h
* @brief a class help to load image
*/
#ifndef SUI_IN_IMAGE_H
#define SUI_IN_IMAGE_H
#include <string>
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SUI_in_canvas.h"
#include "SUI_geometry.h"

namespace sui {

class Sketch : public Geometry {
public:
    Sketch(int posX, int posY, int width, int height);
    bool load_sketch(const std::string &image_file);
    void destroy_sketch();
    void draw_sketch(Canvas &canvas);
    unsigned get_sketch_width() const;
    unsigned get_sketch_height() const;
    void set_source_area(int x, int y, int w, int h);
    ~Sketch();
private:
    SDL_Surface *sketch_surface;
/**
* @todo use texture buffer rathder than surface
*/
    SDL_Texture *sketch_texture;
    SDL_Rect source_area;
    friend Canvas;
};

}
#endif