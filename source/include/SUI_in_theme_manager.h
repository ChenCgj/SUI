#ifndef SUI_IN_THEME_MANAGER_H
#define SUI_IN_THEME_MANAGER_H

#include <vector>
#include "SUI_main.h"
#include "SUI_in_theme.h"
namespace sui {

class Theme_manager {
public:
    static Theme_manager *instance();
    void set_current_theme(Theme_index index);
    Theme &get_theme(Theme_index index);
private:
    Theme_manager();
    Theme_index index;
    std::vector<Theme> all_theme;
};
}
#endif