#ifndef SUI_LABEL_H
#define SUI_LABEL_H

#include "SUI_element.h"

namespace sui {

class Label : public Element {
public:
    Label(const std::string &text = "text", unsigned font_size = 18, int x = 0, int y = 0, int w = 60, int h = 30);
    void draw(Canvas &canvas) override;
    void set_text(const std::string &text);
    const std::string &get_text();
    ~Label();
private:
    std::string text;
    unsigned font_size;
    friend class Event_handler_helper;
};
}
#endif