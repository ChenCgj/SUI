#ifndef SUI_SLIDER_H
#define SUI_SLIDER_H
#include "SUI_element.h"

namespace sui {

class Slider : public Element {
public:
    enum class Slider_event {
        se_slide
    };
    Slider(int x = 0, int y = 0, int w = 100, int h = 30, double min = 0, double max = 100);
    void set_range(double min, double max);
    std::pair<int, int> get_range() const;
    void set_value(double value);
    double get_value() const;
    void add_listener(const std::function<void (const Mouse_motion_event &, void *)> &func, Slider_event event, void *arg);
    void draw(Canvas &canvas) override;
    ~Slider();
private:
    void deal_mouse_move_event(Mouse_motion_event &move_event) override;
    void draw_slide_block(Canvas &canvas);
    Rect get_block_rect() const;
    static constexpr int k_block_width = 10;
    double m_min;
    double m_max;
    double m_index;
    std::function<void (const Mouse_motion_event &, void *)> cb_slide;
    void *a_slide;
    friend class Event_handler_helper;
};
}
#endif