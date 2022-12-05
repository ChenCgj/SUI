#ifndef SUI_DIALOG_H
#define SUI_DIALOG_H

#include "SUI_window_base.h"

namespace sui {

class Dialog : public Window_base {
public:
    Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, int posX, int posY, int flag = Window_flag::window_flag_none);
    Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, int flag = Window_flag::window_flag_none);
    ~Dialog();
// private:
//     std::string message;
};

}
#endif