#ifndef SUI_WINDOW_H
#define SUI_WINDOW_H

#include "SUI_window_base.h"

namespace sui {
class Window : public Window_base {
public:
    Window(const std::string &title, int width, int height, int posX, int posY, int flag = Window_flag::window_flag_none);
    Window(const std::string &title, int width, int height, int flag = Window_flag::window_flag_none);
    ~Window();
};

}
#endif