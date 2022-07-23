#ifndef SUI_UTILS_H
#define SUI_UTILS_H

#include <stdint.h>
#include <utility>

#include "SUI_key_code.h"

namespace sui {

class Keyboard_event;

bool is_mouse_left_button_down();

bool is_mouse_right_button_down();

bool is_mouse_middle_button_down();

bool is_shift_down();

bool is_ctrl_down();

bool is_alt_down();

std::pair<int, int> get_mouse_pos();

Key_code get_key(Keyboard_event &event);

int add_timer(int interval, uint32_t (*func)(uint32_t interval, void *param), void *param);

void remove_timer(int id);
}
#endif