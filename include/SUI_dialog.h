#ifndef SUI_DIALOG_H
#define SUI_DIALOG_H

#include "SUI_window_base.h"

namespace sui {

class Dialog : public Window_base {
public:
    Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, int posX, int posY, Window_flag flag = Window_flag::window_flag_none);
    Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, Window_flag flag = Window_flag::window_flag_none);
// private:
//     std::string message;
};

}
#endif