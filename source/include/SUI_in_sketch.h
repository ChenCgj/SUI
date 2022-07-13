/**
* @file SUI_in_sketch.h
* @brief a class help to load image
*/
#ifndef SUI_IN_SKETCH_H
#define SUI_IN_SKETCH_H
#include <string>
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SUI_in_canvas.h"
#include "SUI_geometry.h"

namespace sui {

class Sketch : public Geometry {
public:
    Sketch(int width, int height);
    bool load_sketch(const std::string &image_file);
    void destroy_sketch();
    void draw_sketch(Canvas &canvas, int posX = 0, int posY = 0);
    unsigned get_sketch_width() const;
    unsigned get_sketch_height() const;
    void set_source_area(int x, int y, int w, int h);
    ~Sketch();
private:
    SDL_Surface *sketch_surface;
    long long image_texture_id;
    // may be doesn't need the texture_id
    long long texture_id;
    SDL_Rect source_area;
    friend Canvas;
};

}
#endif