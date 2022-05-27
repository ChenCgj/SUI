#include "SUI_in_theme.h"
#include "SUI_in_theme_manager.h"
#include "SUI_main.h"
#include "SUI_in_debug.h"
namespace sui {

Theme_manager::Theme_manager() :
    index{default_theme}, all_theme(int(custom_theme) + 1) {}

Theme_manager *Theme_manager::instance() {
    static Theme_manager manager;
    return &manager;
}

Theme &Theme_manager::get_theme(Theme_index index) {
    if (index == current_theme) {
        return all_theme[this->index];
    }
    return all_theme[index];
}

void Theme_manager::set_current_theme(Theme_index index) {
    if (index == current_theme) {
        ERR(<< "set the curren theme error");
        return;
    }
    this->index = index;
}

}