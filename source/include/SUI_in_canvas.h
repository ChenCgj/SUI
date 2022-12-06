
/**
* @file SUI_in_canvas.h
* @brief contains definition of canvas
*/

#ifndef SUI_CANVAS
#define SUI_CANVAS
#include <string>
#include <stack>
#include <vector>

#include "glad/glad.h"

#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

#include "SUI_color.h"
#include "SUI_geometry.h"
#include "SUI_in_main.h"
#include "SUI_shape.h"

namespace sui {

class Window_base;
class Sketch;
/**
* @class Canvas
* @brief this class is designed to draw things on it
* 
* usually, a drawale element will draw itself on a canvas,
* and the window will copy all of its children's canvas on its window area
*
* @warning only can use a canvas to render a window and the texture created by the canvas
*          when you change the window it connect, it should reload and get the render environment
*/

class Canvas : public Geometry {
public:
    /**
    * @fn Canvas
    * @param [in] window the canvas will connect to get the render infomation
    * @param [in] posX
    * @param [in] posY
    * @param [in] width
    * @param [in] height
    * @param [in] depth
    */
    Canvas(const Window_base &window, int posX, int posY, int posZ, int width, int height, int depth);
    /**
    * @fn Canvas
    * when you not provide a window, it will load the render information from it's parent if possible
    */
    Canvas(int posX, int posY, int posZ, int width, int height, int depth, bool for_gl_data = false);
    virtual ~Canvas();
    void draw_line(const Point &first, const Point &second);
    void draw_lines(const std::vector<Point> &points);
    void fill_rect(const Rect &rect);
    void draw_rect(const Rect &rect);
    void draw_arc(const Point &center, double radius, double start_angle, double end_angle);
    void draw_circle(const Point &center, double radius);
    void draw_ellipse_arc(const Point &center, double semiX_axis, double semiY_axis, double start_angle, double end_angle);
    void draw_ellipse(const Point &center, double semiX_axis, double semiY_axis);
    void draw_round_rect(const Rect &rect, double radius);
    void draw_text(const Rect &rect, const std::string &str, const std::string &font_name, const Color &color, unsigned font_size);
    void draw_point(const Point &point);
    void draw_shape(const Shape &shape);
    void fill_shape(const Shape &shape);
    void draw_sketch(const Sketch &image);
    bool load_gl_Texture(GLuint gl_texture_id, const Rect &rect);
    bool check_need_redraw();
    void set_mask_mode(Mask_mode mask);
    void set_need_redraw(bool redraw);
    void set_always_redraw(bool always);
    bool get_always_redraw() const;
    /**
    * @todo add the fill graph funcitons
    */
    /**
    * @fn paint_on_window
    * @param window a window the canvas connect to
    * @warning this function will change the window's render render target to the window and clean all on the window
    */
    void paint_on_window(const Window_base &window);
    /**
    * @fn paint_on_canvas
    * @param canvas the destination to render
    * @brief this function will render the content this have to the canvas' texture
    * @warning the canvas and this should connect to the same window(same render information)
    */
    void paint_on_canvas(Canvas &canvas);
    /**
    * @fn clean
    * @brief clean the render target (such as window or the texture it contains)
    */
    void clear();
    /**
    * @fn set_color
    * @brief set the color the render to draw lines, rects, etc
    */
    void set_color(int r, int g, int b, int a);
    /**
    * @fn isValid
    * @brief check if the canvas has valid texture
    */
    bool isValid();
    /**
    * @fn save_env
    * @brief save the render information such target, color
    * you can call restore_env() to restore
    */
    bool is_for_opengl();
    void save_env();
    /**
    * @fn restore_env
    * @brief resotre the draw information
    * @warning you should call save_env() before
    */
    void restore_env();
    /**
    * @fn load_renderer
    * @brief copy a render from a canvas (typically created by a window), so that this object connect to a window
    */
    void load_renderer(Canvas &canvas);
    /**
    * @fn present
    * @brief flush window
    * @warning only valid when the target is the window
    */
    void present();
    void unload_renderer();
private:
    long long texture_id;
    // SDL_Texture *pTexture;
    SDL_Renderer *pRenderer;

    struct Renderer_env;
    std::stack<Renderer_env*> env_stack;
    Mask_mode mask_mode;
    bool need_redraw;
    bool always_redraw;
    bool for_gl_data;
    // backup the size, so that when size change we can recreate new content
    int width_bak, height_bak, depth_bak;
    // detect if the size change and realloc the new content
    bool prepare_texture();
    // the draw color
    int r, g, b, a;
};
}
#endif