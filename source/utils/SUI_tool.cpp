#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_in_window_manager.h"
#include "SUI_tool.h"

namespace sui {

void present_all() {
    WINDOW_MANAGER->update_all_window();
}

}