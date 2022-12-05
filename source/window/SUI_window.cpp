#include "SUI_window.h"

namespace sui {

Window::Window(const std::string &title, int width, int height, int posX, int posY, int flag)
    : Geometry{0, 0, width, height}, Window_base{title, width, height, posX, posY, flag} {
    object_name = "window";
}

Window::Window(const std::string &title, int width, int height, int flag)
    : Geometry{0, 0, width, height}, Window_base{title, width, height, flag} {
    object_name = "window";
}

Window::~Window() {
    prepare_destroy();
}
}