#ifndef SUI_TOOL_H
#define SUI_TOOL_H
#include "SUI_theme_index.h"
#include "SUI_window.h"
#include "SUI_run_mode.h"

namespace sui {

void present_all();
void present(Window *pWindow);
void set_theme(Theme_index index);
void quit();
void register_clean(void (*func)());
void set_run_mode(Run_mode mode);
}

#endif