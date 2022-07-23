#include "SDL_ttf.h"
#include "SDL_image.h"

#include "SUI_in_managers.h"
#include "SUI_in_timer_manager.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_texture_sdl_manager.h"
#include "SUI_in_theme_manager.h"
#include "SUI_object.h"

namespace sui {

Managers::Managers() :
    pwm{Window_manager::instance()},
    ptsm{Texture_sdl_manager::instance()},
    ptm{Theme_manager::instance()},
    root{Object::root_instance()},
    trash_root{Object::trash_root_instance()},
    tm{Timer_manager::instance()} {}

Managers::~Managers() {
    delete tm;
    delete trash_root;
    delete root;
    delete ptm;
    delete ptsm;
    delete pwm;
    antinit();
}

void Managers::init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_PNG | IMG_INIT_WEBP);
}

void Managers::antinit() {
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

Object *Managers::get_root() {
    return root;
}

Object *Managers::get_trash_root() {
    return trash_root;
}

Theme_manager *Managers::get_theme_manager() {
    return ptm;
}

Texture_sdl_manager *Managers::get_texture_sdl_manager() {
    return ptsm;
}

Window_manager *Managers::get_window_manager() {
    return pwm;
}

Timer_manager *Managers::get_timer_manager() {
    return tm;
}

Managers *Managers::instance() {
    static Managers managers;
    return &managers;
}
}