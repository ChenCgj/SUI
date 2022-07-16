#include "SUI_in_managers.h"
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
    trash_root{Object::trash_root_instance()} {}

Managers::~Managers() {
    delete trash_root;
    delete root;
    delete ptm;
    delete ptsm;
    delete pwm;
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

Managers *Managers::instance() {
    static Managers managers;
    return &managers;
}
}