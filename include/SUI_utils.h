#ifndef SUI_UTILS_H
#define SUI_UTILS_H

#include <stdint.h>
#include <utility>

#include "SUI_key_code.h"

namespace sui {

class Keyboard_event;
class Mouse_wheel_event;
class Mouse_motion_event;

bool is_mouse_left_button_down();

bool is_mouse_right_button_down();

bool is_mouse_middle_button_down();

std::pair<int, int> get_mouse_pos();

bool is_mouse_left_button_down(const Mouse_motion_event &e);

bool is_mouse_middle_button_down(const Mouse_motion_event &e);

bool is_mouse_right_button_down(const Mouse_motion_event &e);

std::pair<int ,int> get_mouse_move(const Mouse_motion_event &e);

std::pair<int, int> get_mouse_pos(const Mouse_motion_event &e);

float get_wheel_scroll(const Mouse_wheel_event &event);

bool is_shift_down();

bool is_ctrl_down();

bool is_alt_down();

const bool *get_key_state(int &num_keys);

Key_code get_key(const Keyboard_event &event);

int add_timer(int interval, uint32_t (*func)(uint32_t interval, void *param), void *param);

void remove_timer(int id);
}
#endif