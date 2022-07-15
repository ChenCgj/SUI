#ifndef SUI_IN_MANAGERS_H
#define SUI_IN_MANAGERS_H

#include "SUI_in_texture_sdl_manager.h"
#include "SUI_in_theme_manager.h"
#include "SUI_in_window_manager.h"
#include "SUI_object.h"
namespace sui {

class Managers {
public:
    static Managers *instance();
    Window_manager *get_window_manager();
    Texture_sdl_manager *get_texture_sdl_manager();
    Theme_manager *get_theme_manager();
    Object *get_root();
    Object *get_trash_root();
    ~Managers();
private:
    Managers();
    Window_manager *pwm;
    Texture_sdl_manager *ptsm;
    Theme_manager *ptm;
    Object *root;
    Object *trash_root;
};
}
#endif