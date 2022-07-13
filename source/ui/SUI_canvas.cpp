#include "SDL_blendmode.h"
#include "SDL_error.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

#include "SUI_in_canvas.h"
#include "SUI_in_main.h"
#include "SUI_in_window_data.h"
#include "SUI_in_debug.h"
#include "SUI_in_sketch.h"
#include "SUI_in_texture_sdl_manager.h"

namespace sui {

struct Canvas::Renderer_env {
    SDL_Texture *pOrigin_target;
    Uint8 o_r, o_g, o_b, o_a;
};

// static void store_env(SDL_Renderer *pRenderer, SDL_Texture *env_pOrigin_target, Uint8 o_r, Uint8 o_g, Uint8 o_b, Uint8 o_a);
// static void load_env(SDL_Renderer *pRenderer, SDL_Texture *env_pOrigin_target, Uint8 o_r, Uint8 o_g, Uint8 o_b, Uint8 o_a);
static SDL_Texture *recreate_texture(SDL_Renderer *pRenderer, SDL_Texture *origin, int origin_w, int origin_h, int new_w, int new_h);
static const double math_pi = 4 * atan(1);
static SDL_BlendMode mask_blend_mode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_COLOR, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
Canvas::Canvas(const Window &window, int posX, int posY, int posZ, int width, int height, int depth)
    : Canvas(posX, posY, posZ, width, height, depth) {

    // load the render from a window
    pRenderer = window.pData->pRenderer;
    // pTexture = SDL_CreateTexture(pRenderer,
    //     SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    TEXTURE_SDL_MANAGER->set_texture(texture_id, pRenderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    // if (pTexture == nullptr) {
    //     ERR(<< "couldn't create the texutre");
    // }
    // should set the blend mode so that we can have the transparent effective
    // SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
}

Canvas::Canvas(int posX, int posY, int posZ, int width, int height, int depth)
    : Geometry{posX, posY, posZ, width, height, depth}, texture_id{TEXTURE_SDL_MANAGER->alloc_texture_id()}, pRenderer{nullptr}, mask_mode{Mask_mode::none_mask},
    need_redraw{true}, width_bak{width}, height_bak{height}, depth_bak{depth}, r{0}, g{0}, b{0}, a{255} {}

/**
* Should not use the save_env with not restore_env before this function
* for this function will not update the texture in the env stack
* @todo update the env stack
*/
void Canvas::load_renderer(Canvas &canvas) {
    // if the render is the same, we confirm the texture is created from the same render
    if (pRenderer == canvas.pRenderer) {
        return;
    }
    pRenderer = canvas.pRenderer;
    TEXTURE_SDL_MANAGER->set_texture(texture_id, pRenderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, get_width(), get_height());
    // if (pTexture) {
    //     SDL_DestroyTexture(pTexture);
    // }
    // pTexture = SDL_CreateTexture(pRenderer,
    //     SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, get_width(), get_height());
    // if (pTexture == nullptr) {
    //     ERR(<< "nullptr texture: SDL: " << SDL_GetError());
    // }
    // // should set the blend mode so that we can have the transparent effective
    // SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
    need_redraw = true;
}

void Canvas::unload_renderer() {
    // if (pTexture) {
    //     SDL_DestroyTexture(pTexture);
    //     pTexture = nullptr;
    // }
    TEXTURE_SDL_MANAGER->set_texture(texture_id, nullptr, (SDL_Texture *)nullptr);
    pRenderer = nullptr;
}

Canvas::~Canvas() {
    unload_renderer();
    while (!env_stack.empty()) {
        delete env_stack.top();
        env_stack.pop();
        ERR(<< "the env stack is not empty");
    }
}

void Canvas::draw_point(const Point &point) {
    if (!prepare_texture()) {
        ERR(<< "couldn't prepare the texture.");
        return;
    }
    if (SDL_RenderDrawPointF(pRenderer, point.x, point.y) < 0) {
        ERR(<< "draw point failure. SDL: " << SDL_GetError());
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
    if (SDL_RenderDrawLineF(pRenderer, first.x, first.y, second.x, second.y) < 0) {
        ERR(<< "draw line failure. SDL: " << SDL_GetError());
    }
}

void Canvas::draw_lines(const std::vector<Point> &points) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_FPoint *sdl_points = new SDL_FPoint[points.size()];
    if (!sdl_points) {
        ERR(<< "draw lines failure");
        return;
    }
    for (int i = 0; i < points.size(); ++i) {
        sdl_points[i] = SDL_FPoint{static_cast<float>(points[i].x), static_cast<float>(points[i].y)};
    }
    if (SDL_RenderDrawLinesF(pRenderer, sdl_points, points.size()) < 0) {
        ERR(<< "draw line failure. SDL: " << SDL_GetError());
    }
    delete [] sdl_points;
}

void Canvas::fill_rect(const Rect &rect) {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_FRect rect_in = {static_cast<float>(rect.p1.x), static_cast<float>(rect.p1.y), static_cast<float>(rect.get_width()), static_cast<float>(rect.get_height())};
    if (SDL_RenderFillRectF(pRenderer, &rect_in) < 0) {
        ERR(<< "renderer error, SDL: " << SDL_GetError());
    }
}

void Canvas::draw_rect(const Rect &rect) {
    if (!prepare_texture()) {
        ERR(<< "couldn't prepare the texture.");
        return;
    }
    SDL_FRect rect_in = {static_cast<float>(rect.p1.x), static_cast<float>(rect.p1.y), static_cast<float>(rect.get_width()), static_cast<float>(rect.get_height())};
    if (SDL_RenderDrawRectF(pRenderer, &rect_in) < 0) {
        ERR(<< "Error in draw rect. SDL:" << SDL_GetError());
    }
}

void Canvas::draw_ellipse_arc(const Point &center, double semiX_axis, double semiY_axis, double start_angle, double end_angle) {
    if (semiX_axis <= 0 || semiY_axis <= 0) {
        ERR(<< "the axis of ellipse is zero!");
        return;
    }
    if (!prepare_texture()) {
        ERR(<< "couldn't prepare the texture.");
        return;
    }
    constexpr int count = 100;
    SDL_FPoint points[count], temp;

    double delta = std::min(0.001, 0.02 / (semiX_axis + semiY_axis));
    delta = end_angle > start_angle ? delta : -delta;

    double angle = start_angle;
    bool first = true;
    while (abs(angle - end_angle) >= delta) {
        int i = 1;
        for (i = 1; abs(angle - end_angle) >= delta && i < count; angle += delta, ++i) {
            points[i].x = center.x + semiX_axis * cos(angle);
            points[i].y = center.y - semiY_axis * sin(angle);
        }
        if (first) {
            points[0] = points[1];
            first = false;
        } else {
            points[0] = temp;
        }
        temp = points[i - 1];
        if (SDL_RenderDrawLinesF(pRenderer, points, i) < 0) {
            ERR(<< "Error in draw ellipse arc. SDL:" << SDL_GetError());
        }
    }
}

void Canvas::draw_ellipse(const Point &center, double semiX_axis, double semiY_axis) {
    draw_ellipse_arc(center, semiX_axis, semiY_axis, 0, 2 * math_pi);
}

void Canvas::draw_arc(const Point &center, double radius, double start_angle, double end_angle) {
    draw_ellipse_arc(center, radius, radius, start_angle, end_angle);
}

void Canvas::draw_circle(const Point &center, double radius) {
    draw_ellipse(center, radius, radius);
}

void Canvas::draw_shape(const Shape &shape) {
    shape.draw_shape(*this);
}

void Canvas::fill_shape(const Shape &shape) {
    shape.fill_shape(*this);
}

void Canvas::draw_sketch(const Sketch &image) {
    SDL_Texture *texture = TEXTURE_SDL_MANAGER->get_texture(image.texture_id);

    if (!TEXTURE_SDL_MANAGER->is_valid(image.texture_id) || pRenderer != TEXTURE_SDL_MANAGER->get_renderer(image.texture_id)) {
        SDL_Texture *image_texture = TEXTURE_SDL_MANAGER->get_texture(image.image_texture_id);
        if (!TEXTURE_SDL_MANAGER->is_valid(image.image_texture_id) || pRenderer != TEXTURE_SDL_MANAGER->get_renderer(image.image_texture_id)) {
            if (image.sketch_surface != nullptr) {
                image_texture = TEXTURE_SDL_MANAGER->set_texture(image.image_texture_id, pRenderer, image.sketch_surface);
                if (image_texture == nullptr) {
                    ERR(<< "create texture fail." << SDL_GetError());
                    return;
                }
            } else {
                image_texture = nullptr;
            }
        }
        Uint32 format;
        int width, height;

        if (image_texture) {
            SDL_QueryTexture(image_texture, &format, nullptr, &width, &height);
        } else {
            SDL_RendererInfo info;
            SDL_GetRendererInfo(pRenderer, &info);
            width = image.get_width();
            height = image.get_height();
            if (info.num_texture_formats == 0) {
                ERR(<< "the renderer supported no format.");
                return;
            }
            format = info.texture_formats[0];
        }
        texture = TEXTURE_SDL_MANAGER->set_texture(image.texture_id, pRenderer, 
            format, SDL_TEXTUREACCESS_TARGET, width, height);

        if (texture == nullptr) {
            ERR(<< "create texture fail. " << SDL_GetError());
        }
        save_env();
        SDL_SetRenderTarget(pRenderer, texture);
        if (image_texture) {
            SDL_RenderCopy(pRenderer, image_texture, nullptr, nullptr);
        } else {
            SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
            SDL_RenderClear(pRenderer);
        }
        SDL_RenderPresent(pRenderer);
        restore_env();
    }
    if (!prepare_texture()) {
        ERR(<< "couldn't prepare the texture.");
        return;
    }
    SDL_Rect rect_dst = {image.get_posX(), image.get_posY(), image.get_width(), image.get_height()};
    SDL_Rect rect_src = image.source_area;
    if (rect_src.h == 0) {
        rect_src.y = 0;
        rect_src.h = image.get_sketch_height();
        rect_src.h = rect_src.h ? rect_src.h : image.get_height();
    }
    if (rect_src.w == 0) {
        rect_src.x = 0;
        rect_src.w = image.get_sketch_width();
        rect_src.w = rect_src.w ? rect_src.w : image.get_width();
    }
    DBG(<< "src: " << rect_src.x << " " << rect_src.y << " " << rect_src.w << " " << rect_src.h);
    DBG(<< "dest: " << rect_dst.x << " " << rect_dst.y << " " << rect_dst.w << " " << rect_dst.h);
    if (SDL_RenderCopy(pRenderer, texture, &rect_src, &rect_dst) < 0) {
        ERR(<< "render copy error. SDL: " << SDL_GetError());
    }

    SDL_RenderPresent(pRenderer);
}

void Canvas::draw_round_rect(const Rect &rect, double radius) {
    radius = std::min({fabs(rect.get_width()) / 2, fabs(rect.get_height()) / 2, radius});
    Point p1 = rect.p1;
    Point p2 = rect.p2;
    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
    }
    if (p1.y > p2.y) {
        std::swap(p1.y, p2.y);
    }
    draw_line(Point{p1.x + radius, p1.y, 0}, Point{p2.x - radius, p1.y, 0});
    draw_line(Point{p1.x, p1.y + radius, 0}, Point{p1.x, p2.y - radius, 0});
    /**
    * @todo maybe you should use the drawlineF? sub 1 is to make the line not be covered
    */
    draw_line(Point{p1.x + radius, p2.y - 1, 0}, Point{p2.x - radius, p2.y - 1, 0});
    draw_line(Point{p2.x - 1, p1.y + radius, 0}, Point{p2.x - 1, p2.y - radius, 0});
    draw_arc(Point{p1.x + radius, p1.y + radius, 0}, radius, math_pi / 2, math_pi);
    draw_arc(Point{p2.x - radius, p1.y + radius, 0}, radius, 0, math_pi / 2);
    draw_arc(Point{p1.x + radius, p2.y - radius, 0}, radius, math_pi, math_pi * 3 / 2);
    draw_arc(Point{p2.x - radius, p2.y - radius, 0}, radius, math_pi * 3 / 2, math_pi * 2);
}

void Canvas::draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned font_size) {
    TTF_Font *font = TTF_OpenFont(font_name.c_str(), font_size);
    if (font == nullptr) {
        ERR(<< "open font fail! TTF:" << TTF_GetError());
        return;
    }
    SDL_Color font_color = {color.red, color.green, color.blue, color.alpha};
    SDL_Surface *font_sf = TTF_RenderUTF8_Blended(font, str.c_str(), font_color);
    if (font_sf == nullptr) {
        ERR(<< "create font surface fail. TTF:" << TTF_GetError());
        TTF_CloseFont(font);
        return;
    }
    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(pRenderer, font_sf);
    if (font_texture == nullptr) {
        ERR(<< "create font texture fail. SDL:" << SDL_GetError());
        SDL_FreeSurface(font_sf);
        TTF_CloseFont(font);
        return;
    }
    SDL_FreeSurface(font_sf);
    SDL_Rect rsrc = {0, 0, 0, 0};
    TTF_SizeText(font, str.c_str(), &rsrc.w, &rsrc.h);
    TTF_CloseFont(font);
    SDL_FRect rdest = {static_cast<float>(rect.p1.x + (rect.get_width() - rsrc.w) / 2), static_cast<float>(rect.p1.y + (rect.get_height() - rsrc.h) / 2), static_cast<float>(rsrc.w), static_cast<float>(rsrc.h)};
    SDL_RenderCopyF(pRenderer, font_texture, &rsrc, &rdest);
    SDL_DestroyTexture(font_texture);
}

void Canvas::set_mask_mode(Mask_mode mask) {
    mask_mode = mask;
}

void Canvas::set_need_redraw(bool redraw) {
    need_redraw = redraw;
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
    SDL_Texture *pTexture = TEXTURE_SDL_MANAGER->get_texture(texture_id);
    if (mask_mode == Mask_mode::to_mask) {
        SDL_BlendMode mode_bak = SDL_BLENDMODE_INVALID;
        SDL_GetTextureBlendMode(pTexture, &mode_bak);
        SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_MOD);
        if (SDL_RenderCopy(window.pData->pRenderer, pTexture, &rect_src, &rect_dst) < 0) {
            ERR(<< "render copy error. SDL: " << SDL_GetError());
        }
        SDL_SetTextureBlendMode(pTexture, mask_blend_mode);
        if (SDL_RenderCopy(window.pData->pRenderer, pTexture, &rect_src, &rect_dst) < 0) {
            ERR(<< "render copy error. SDL: " << SDL_GetError());
        }
        SDL_SetTextureBlendMode(pTexture, mode_bak);
    } else if (SDL_RenderCopy(window.pData->pRenderer, pTexture, &rect_src, &rect_dst) < 0) {
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
    SDL_Texture *pTexture = TEXTURE_SDL_MANAGER->get_texture(texture_id);
    if (mask_mode == Mask_mode::to_mask || canvas.mask_mode == Mask_mode::be_masked) {
        SDL_BlendMode mode_bak = SDL_BLENDMODE_INVALID;
        SDL_GetTextureBlendMode(pTexture, &mode_bak);
        SDL_SetTextureBlendMode(pTexture, mask_blend_mode);
        if (SDL_RenderCopy(canvas.pRenderer, pTexture, &rect_src, &rect_dst) < 0) {
            ERR(<< "render copy error. SDL: " << SDL_GetError());
        }
        SDL_SetTextureBlendMode(pTexture, mode_bak);
    } else if (SDL_RenderCopy(canvas.pRenderer, pTexture, &rect_src, &rect_dst) < 0) {
        ERR(<< "render copy error. SDL: " << SDL_GetError());
    };
    /**
    * may call this function is uncessary? but it fix the bug that when the object too more, the window can't show some element on windows
    * may be this function make all buffer to the texture?
    */
    SDL_RenderPresent(canvas.pRenderer);
// debug
    // extern SDL_Window *pw;
    // extern SDL_Renderer *pr;
    // int r = 0;
    // r = SDL_SetRenderTarget(pr, nullptr);
    // r = SDL_SetRenderDrawColor(pr, 255, 255, 255, 255);
    // r = SDL_RenderClear(pr);
    // SDL_RenderPresent(pr);
    // r = SDL_RenderCopy(pr, pTexture, nullptr, nullptr);
    // SDL_RenderPresent(pr);
    // r = SDL_RenderClear(pr);
    // SDL_RenderPresent(pr);
    // SDL_Texture *cp = TEXTURE_SDL_MANAGER->get_texture(canvas.texture_id);
    // r = SDL_RenderCopy(pr, cp, nullptr, nullptr);
    // SDL_RenderPresent(pr);
// debug
    canvas.restore_env();
}

/**
* this function will realloc the texture if nexxessary and update the env stack
*/
bool Canvas::prepare_texture() {
    SDL_Texture *pTexture = TEXTURE_SDL_MANAGER->get_texture(texture_id);
    if (pRenderer == nullptr || pTexture == nullptr) {
        ERR(<< "the renderer is not valid!");
        return false;
    }
    // if the size has changed
    if (!isValid()) {
        DBG(<< "need to create a new texture...");
        save_env();
        SDL_Texture *new_texture = recreate_texture(pRenderer, pTexture, width_bak, height_bak, get_width(), get_height());
        TEXTURE_SDL_MANAGER->set_texture(texture_id, pRenderer, new_texture);
        if (new_texture == nullptr) {
            ERR(<< "new texture create fail!");
            restore_env();
            return false;
        }
        std::stack<Renderer_env *> env_bak;
        while (!env_stack.empty()) {
            Renderer_env *e = env_stack.top();
            env_stack.pop();
            // when we use SDL_SetRendererTarget(renderer, texture) and then get the target using SDL_GetRenderTarget(renderer)
            // it was finded that the **target = texture + 0x98** instead of **target == texture**
            // so we make the target which is not nullptr in stack point to the new texture
            // however, we wouldn't use this canvas render anything but the window and the texture in the canvas itself.
            if (e->pOrigin_target/* == pTexture*/) {
                e->pOrigin_target = new_texture;
            }
            env_bak.push(e);
        }
        while (!env_bak.empty()) {
            env_stack.push(env_bak.top());
            env_bak.pop();
        }
        restore_env();
        pTexture = new_texture;
        width_bak = get_width();
        height_bak = get_height();
        depth_bak = get_depth();
        need_redraw = true;
    }
    return true;
}

void Canvas::present() {
    SDL_RenderPresent(pRenderer);
}

void Canvas::clear() {
    if (!prepare_texture()) {
        ERR(<< "couldn't preapre the texture.");
        return;
    }
    SDL_RenderClear(pRenderer);
}

void Canvas::set_color(int r, int g, int b, int a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    SDL_SetRenderDrawColor(pRenderer, this->r, this->g, this->b, this->a);
}

bool Canvas::check_need_redraw() {
    return need_redraw;
}

bool Canvas::isValid() {
    return TEXTURE_SDL_MANAGER->is_valid(texture_id) && get_width() == width_bak && get_height() == height_bak && get_depth() == depth_bak;
}

void Canvas::save_env() {
    Renderer_env *env = new Renderer_env;
    env->pOrigin_target = nullptr;
    env->o_r = env->o_g = env->o_b = env->o_a = 0;
    // store_env(pRenderer, env);
    if (pRenderer != nullptr) {
        SDL_GetRenderDrawColor(pRenderer, &env->o_r, &env->o_g, &env->o_b, &env->o_a);
        env->pOrigin_target = SDL_GetRenderTarget(pRenderer);
    }
    env_stack.push(env);
    if (env_stack.size() > 1) {
        DBG(<< "WARNING: some canvas push two env!");
    }
    SDL_Texture *pTexture = TEXTURE_SDL_MANAGER->get_texture(texture_id);
    if (SDL_SetRenderTarget(pRenderer, pTexture) == -1) {
        ERR(<< "couldn't set the target. SDL: " << SDL_GetError());
    }
    // notice that tht target we get and we set is not equal!!!
    // printf("the set texture is %p, and the get texture is %p\n", pCanvas_data->pTexture, SDL_GetRenderTarget(pCanvas_data->pRenderer));
}

void Canvas::restore_env() {
    Renderer_env *env = env_stack.top();
    env_stack.pop();
    if (pRenderer != nullptr) {
        set_color(env->o_r, env->o_g, env->o_b, env->o_a);
        if (SDL_SetRenderTarget(pRenderer, env->pOrigin_target) == -1) {
            ERR(<< "Set target failure " << SDL_GetError());
        }
    }
    delete env;
}

// void store_env(SDL_Renderer *pRenderer, Canvas::Renderer_env &env) {
//     if (pRenderer == nullptr) {
//         return;
//     }
//     SDL_GetRenderDrawColor(pRenderer, &env.o_r, &env.o_g, &env.o_b, &env.o_a);
//     env.pOrigin_target = SDL_GetRenderTarget(pRenderer);
// }

// void load_env(SDL_Renderer *pRenderer, Canvas::Renderer_env &env) {
//     if (pRenderer == nullptr) {
//         return;
//     }
//     if (SDL_SetRenderDrawColor(pRenderer, env.o_r, env.o_g, env.o_b, env.o_a) == -1) {
//         ERR(<< "Set color failure" << SDL_GetError());
//     }
//     if (SDL_SetRenderTarget(pRenderer, env.pOrigin_target) == -1) {
//         ERR(<< "Set target failure" << SDL_GetError());
//     }
// }

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
    // if (SDL_RenderCopy(pRenderer, origin, &rect_src, &rect_src) < 0) {
    //     ERR(<< "render copy origin texture fail. SDL: " << SDL_GetError());
    // }
    // SDL_DestroyTexture(origin);
    return new_texture;
}
}