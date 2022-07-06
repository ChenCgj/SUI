#ifndef SUI_IN_CANVAS_DATA_H
#define SUI_IN_CANVAS_DATA_H

#include <stack>
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "SUI_in_canvas.h"

namespace sui {

struct Renderer_env {
    SDL_Texture *pOrigin_target;
    Uint8 o_r, o_g, o_b, o_a;
};

struct Canvas::Canvas_data {
    SDL_Texture *pTexture;
    SDL_Renderer *pRenderer;

    Renderer_env env;
    std::stack<Renderer_env> env_stack;
};

}
#endif