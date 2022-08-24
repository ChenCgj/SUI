#ifndef SUI_TEXT_AREA_H
#define SUI_TEXT_AREA_H

#include "SUI_element.h"

namespace sui {

class Text_area : public Element {
public:
    Text_area(const std::string &text = "text", unsigned font_size = 18, int x = 0, int y = 0, int w = 60, int h = 30);
    void draw(Canvas &canvas) override;
    void set_text(const std::string &text);
    const std::string &get_text();
    ~Text_area();
private:
    std::string text;
    unsigned font_size;
    friend class Event_handler_helper;
};
}
#endif