#include <cstddef>
#include <iostream>
#include <memory>

#include "SDL_blendmode.h"
#include "SDL_error.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"

#include "SUI_canvas.h"
#include "SUI_in_canvas_data.h"
#include "SUI_in_window_data.h"
#include "SUI_in_debug.h"

namespace sui {

static void store_env(SDL_Renderer *pRenderer, Renderer_env &env);
static void load_env(SDL_Renderer *pRenderer, Renderer_env &env);
static SDL_Texture *recreate_texture(SDL_Renderer *pRenderer, SDL_Texture *origin, int origin_w, int origin_h, int new_w, int new_h);

Canvas::Canvas(const Window &window, int posX, int posY, int posZ, int width, int height, int depth)
    : Canvas(posX, posY, posZ, width, height, depth) {

    // load the render from a window
    pCanvas_data->pRenderer = window.pData->pRenderer;
    pCanvas_data->pTexture = SDL_CreateTexture(pCanvas_data->pRenderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    // should set the blend mode so that we can have the transparent effective
    SDL_SetTextureBlendMode(pCanvas_data->pTexture, SDL_BLENDMODE_BLEND);
}
 
Canvas::Canvas(int posX, int posY, int posZ, int width, int height, int depth)
    : Geometry{posX, posY, posZ, width, height, depth},
    pCanvas_data{nullptr}, width_bak{width}, height_bak{height}, depth_bak{depth} {

    pCanvas_data = std::make_unique<Canvas_data>();
    pCanvas_data->pRenderer = nullptr;
    pCanvas_data->pTexture = nullptr;
}

/**
* Should not use the save_env with not restore_env before this function
* for this function will not update the texture in the env stack
* @todo update the env stack
*/
void Canvas::load_renderer(Canvas &canvas) {
    // if the render is the same, we confirm the texture is created from the same render
    if (pCanvas_data->pRenderer == canvas.pCanvas_data->pRenderer) {
        return;
    }
    pCanvas_data->pRenderer = canvas.pCanvas_data->pRenderer;
    if (pCanvas_data->pTexture) {
        SDL_DestroyTexture(pCanvas_data->pTexture);
    }
    pCanvas_data->pTexture = SDL_CreateTexture(pCanvas_data->pRenderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET, get_width(), get_height());
    if (pCanvas_data->pTexture == nullptr) {
        ERR(<< "nullptr texture: SDL: " << SDL_GetError());
    }
    // should set the blend mode so that we can have the transparent effective
    SDL_SetTextureBlendMode(pCanvas_data->pTexture, SDL_BLENDMODE_BLEND);
}

Canvas::~Canvas() {
    if (pCanvas_data->pTexture) {
        SDL_DestroyTexture(pCanvas_data->pTexture);
    }
}
/**
* @brief
* @warning when call this functions, should prepare the correct target, window or the texture
*/
void Canvas::draw_line(const Point &first, const Point &second) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    if (SDL_RenderDrawLine(pCanvas_data->pRenderer, first.x, first.y, second.x, second.y) < 0) {
        printf("%s\n", SDL_GetError());
    }
}

void Canvas::fill_rect(const Rect &rect) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_Rect rect_in = {rect.p1.x, rect.p1.y, rect.get_width(), rect.get_height()};
    if (SDL_RenderFillRect(pCanvas_data->pRenderer, &rect_in) < 0) {
        ERR(<< "renderer error, SDL: " << SDL_GetError());
    }
}

void Canvas::draw_rect(const Rect &rect) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_Rect rect_in = {rect.p1.x, rect.p1.y, rect.get_width(), rect.get_height()};
    if (SDL_RenderDrawRect(pCanvas_data->pRenderer, &rect_in) < 0) {
        std::cerr << "Error in draw rect. :" << SDL_GetError() << std::endl;
    }
}
/**
* @warning the canvas should connect to the window or renderer
* @warning this function will clean all on the window
*/
void Canvas::paint_on_window(const Window &window) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_Rect rect_dst = {get_posX(), get_posY(), get_width(), get_height()};
    SDL_Rect rect_src = {0, 0, get_width(), get_height()};
    DBG(<< "src: " << rect_src.x << " " << rect_src.y << " " << rect_src.w << " " << rect_src.h);
    DBG(<< "dest: " << rect_dst.x << " " << rect_dst.y << " " << rect_dst.w << " " << rect_dst.h);
    SDL_SetRenderTarget(window.pData->pRenderer, nullptr);
    SDL_SetRenderDrawColor(window.pData->pRenderer, 0, 0, 0, 0);
    SDL_RenderClear(window.pData->pRenderer);
    if (SDL_RenderCopy(window.pData->pRenderer, pCanvas_data->pTexture, &rect_src, &rect_dst) < 0) {
        ERR(<< "render copy error. SDL: " << SDL_GetError());
    };
    SDL_RenderPresent(window.pData->pRenderer);
}

/**
* @warning the canvas should connect to the same window or renderer
*/
void Canvas::paint_on_canvas(Canvas &canvas) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_Rect rect_dst = {get_posX(), get_posY(), get_width(), get_height()};
    SDL_Rect rect_src = {0, 0, get_width(), get_height()};
    DBG(<< "src: " << rect_src.x << " " << rect_src.y << " " << rect_src.w << " " << rect_src.h);
    DBG(<< "dest: " << rect_dst.x << " " << rect_dst.y << " " << rect_dst.w << " " << rect_dst.h);
    canvas.save_env();
    if (SDL_RenderCopy(canvas.pCanvas_data->pRenderer, pCanvas_data->pTexture, &rect_src, &rect_dst) < 0) {
        ERR(<< "render copy error. SDL: " << SDL_GetError());
    }
    canvas.restore_env();
}

/**
* this function will realloc the texture if nexxessary and update the env stack
*/
bool Canvas::prepare_texture() {
    if (!isValid()) {
        ERR(<< " the texture is not valid!");
        return false;
    }
    // if the size has changed
    if (get_width() != width_bak || get_height() != height_bak || get_depth() != depth_bak) {
        DBG(<< "need to create a new texture...");
        save_env();
        SDL_Texture *new_texture = recreate_texture(pCanvas_data->pRenderer, pCanvas_data->pTexture, width_bak, height_bak, get_width(), get_height());
        if (new_texture == nullptr) {
            ERR(<< "new texture create fail!");
            restore_env();
            return false;
        }
        std::stack<Renderer_env> env_bak;
        while (!pCanvas_data->env_stack.empty()) {
            Renderer_env e = pCanvas_data->env_stack.top();
            pCanvas_data->env_stack.pop();
            // when we use SDL_SetRendererTarget(renderer, texture) and then get the target using SDL_GetRenderTarget(renderer)
            // it was finded that the **target = texture + 0x98** instead of **target == texture**
            // so we make the target which is not nullptr in stack point to the new texture
            // however, we wouldn't use this canvas render anything but the window and the texture in the canvas itself.
            if (e.pOrigin_target/* == pCanvas_data->pTexture*/) {
                e.pOrigin_target = new_texture;
            }
            env_bak.push(e);
        }
        while (!env_bak.empty()) {
            pCanvas_data->env_stack.push(env_bak.top());
            env_bak.pop();
        }
        restore_env();
        pCanvas_data->pTexture = new_texture;
        width_bak = get_width();
        height_bak = get_height();
        depth_bak = get_depth();
    }
    return true;
}

void Canvas::present() {
    SDL_RenderPresent(pCanvas_data->pRenderer);
}

void Canvas::clear() {
    SDL_RenderClear(pCanvas_data->pRenderer);
}

void Canvas::set_color(int r, int g, int b, int a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    SDL_SetRenderDrawColor(pCanvas_data->pRenderer, this->r, this->g, this->b, this->a);
}

bool Canvas::isValid() {
    return pCanvas_data->pTexture != nullptr; 
}

void Canvas::save_env() {
    store_env(pCanvas_data->pRenderer, pCanvas_data->env);
    pCanvas_data->env_stack.push(pCanvas_data->env);
    if (pCanvas_data->env_stack.size() > 1) {
        DBG(<< "WARNING: some canvas push two env!");
    }
    if (SDL_SetRenderTarget(pCanvas_data->pRenderer, pCanvas_data->pTexture) == -1) {
        ERR(<< "couldn't set the target. SDL: " << SDL_GetError());
    }
    // notice that tht target we get and we set is not equal!!!
    // printf("the set texture is %p, and the get texture is %p\n", pCanvas_data->pTexture, SDL_GetRenderTarget(pCanvas_data->pRenderer));
}

void Canvas::restore_env() {
    pCanvas_data->env = pCanvas_data->env_stack.top();
    pCanvas_data->env_stack.pop();
    load_env(pCanvas_data->pRenderer, pCanvas_data->env);
}

void store_env(SDL_Renderer *pRenderer, Renderer_env &env) {
    if (pRenderer == nullptr) {
        return;
    }
    SDL_GetRenderDrawColor(pRenderer, &env.o_r, &env.o_g, &env.o_b, &env.o_a);
    env.pOrigin_target = SDL_GetRenderTarget(pRenderer);
}

void load_env(SDL_Renderer *pRenderer, Renderer_env &env) {
    if (pRenderer == nullptr) {
        return;
    }
    if (SDL_SetRenderDrawColor(pRenderer, env.o_r, env.o_g, env.o_b, env.o_a) == -1) {
        ERR(<< "Set color failure" << SDL_GetError());
    }
    if (SDL_SetRenderTarget(pRenderer, env.pOrigin_target) == -1) {
        ERR(<< "Set target failure" << SDL_GetError());
    }
}

/**
* @warning this function will chage the environment of the renderer
*/
static SDL_Texture *recreate_texture(SDL_Renderer *pRenderer, SDL_Texture *origin, int origin_w, int origin_h, int new_w, int new_h) {
    SDL_Texture *new_texture = SDL_CreateTexture(pRenderer,SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET, new_w, new_h);
    if (new_texture == nullptr) {
        ERR(<< "create texture fail, SDL: " << SDL_GetError());
        return nullptr;
    }
    // should set the blend mode so that we can have the transparent effective
    SDL_SetTextureBlendMode(new_texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(pRenderer, new_texture);
    // clean the renderer
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
    SDL_RenderClear(pRenderer);
    SDL_Rect rect_src = {0, 0, origin_w, origin_h};
    // copy the content to the new
    /**
    * @warning if the window size change, the origin texture has the invalid content, and copy it to the new is incorrect, object need redraw
    */
    DBG(<< "src and dest: " << rect_src.x << " " << rect_src.y << " " << rect_src.w << " " << rect_src.h);
    if (SDL_RenderCopy(pRenderer, origin, &rect_src, &rect_src) < 0) {
        ERR(<< "render copy origin texture fail");
    }
    SDL_DestroyTexture(origin);
    return new_texture;
}
}