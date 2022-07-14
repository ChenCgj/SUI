#ifndef SUI_UTILS_H
#define SUI_UTILS_H

#include <utility>

bool is_mouse_left_button_down();

bool is_mouse_right_button_down();

bool is_mouse_middle_button_down();

bool is_shift_down();

bool is_ctrl_down();

bool is_alt_down();

std::pair<int, int> get_mouse_pos();

#endif