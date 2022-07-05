/**
* @file SUI_in_image.h
* @brief a class help to load image
*/
#ifndef SUI_IN_IMAGE_H
#define SUI_IN_IMAGE_H
#include <string>
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SUI_canvas.h"
#include "SUI_geometry.h"

namespace sui {

class Image : public Geometry {
public:
    Image(int posX, int posY, int width, int height);
    bool load_image(const std::string &image_file);
    void destroy_image();
    void draw_image(Canvas &canvas);
    ~Image();
private:
    SDL_Surface *image_surface;
/**
* @todo use texture buffer rathder than surface
*/
    SDL_Texture *image_texture;
    friend Canvas;
};

}
#endif