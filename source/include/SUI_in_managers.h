#ifndef SUI_IN_MANAGERS_H
#define SUI_IN_MANAGERS_H

namespace sui {
class Window_manager;
class Texture_sdl_manager;
class Theme_manager;
class Object;
class Timer_manager;

class Managers {
public:
    static Managers *instance();
    Window_manager *get_window_manager();
    Texture_sdl_manager *get_texture_sdl_manager();
    Theme_manager *get_theme_manager();
    Object *get_root();
    Object *get_trash_root();
    Timer_manager *get_timer_manager();
    static void init();
    static void antinit();
    ~Managers();
private:
    Managers();
    Window_manager *pwm;
    Texture_sdl_manager *ptsm;
    Theme_manager *ptm;
    Object *root;
    Object *trash_root;
    Timer_manager *tm;
};
}
#endif