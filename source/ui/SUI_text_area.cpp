#include "SDL_ttf.h"

#include "SUI_in_debug.h"
#include "SUI_in_canvas.h"

#include "SUI_text_area.h"

namespace sui {

Text_area::Text_area(const std::string &text, unsigned font_size, int x, int y, int w, int h) : Geometry{x, y, w, h}, Element{x, y, w, h}, text{text}, font_size{font_size} {
    object_name = "text_area";
}

void Text_area::draw(Canvas &canvas) {
    DBG(<< get_name() << "draw text_area start...");
    canvas.save_env();
    draw_background(canvas);
    draw_border(canvas);
    Color color;
    get_color(color.red, color.green, color.blue, color.alpha);
    TTF_Font *font = TTF_OpenFont("consola.ttf", font_size);
    if (font == nullptr) {
        ERR(<< "open font fail! TTF:" << TTF_GetError());
        return;
    }
    int perline = 0;
    TTF_MeasureUTF8(font, text.c_str(), get_width(), nullptr, &perline);
    int text_w, text_h;
    TTF_SizeText(font, text.c_str(), &text_w, &text_h);
    TTF_CloseFont(font);
    Rect r = {0, static_cast<double>(text_h * 0.2), static_cast<double>(get_width()), static_cast<double>(text_h * 1.2)};
    int curr_pos = 0;
    while (curr_pos <= text.length()) {
        std::string line = text.substr(curr_pos, perline);
        canvas.draw_text(r, line, "consola.ttf", color, font_size);
        curr_pos += perline;
        r.p1.y += text_h * 1.4;
        r.p2.y += text_h * 1.4;
    }
    canvas.restore_env();
    DBG(<< get_name() << "draw text_area ok");
}

Text_area::~Text_area() {
    DBG(<< get_name() << "was destroy.");
}

void Text_area::set_text(const std::string &text) {
    this->text = text;
}

const std::string &Text_area::get_text() {
    return text;
}
}