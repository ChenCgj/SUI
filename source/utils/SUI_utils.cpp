#include "SDL_keyboard.h"
#include "SDL_mouse.h"

#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_in_timer_manager.h"
#include "SUI_in_events.h"
#include "SUI_utils.h"

namespace sui {
bool is_mouse_left_button_down() {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK;
}

bool is_mouse_middle_button_down() {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MMASK;
}

bool is_mouse_right_button_down() {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_RMASK;
}

std::pair<int, int> get_mouse_pos() {
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    return std::make_pair(x, y);
}

bool is_shift_down() {
    int num_keys = 0;
    const Uint8 *key_state = SDL_GetKeyboardState(&num_keys);
    bool lshift = false, rshift = false;
    if (SDL_SCANCODE_LSHIFT < num_keys) {
        lshift = key_state[SDL_SCANCODE_LSHIFT];
    }
    if (SDL_SCANCODE_RSHIFT < num_keys) {
        rshift = key_state[SDL_SCANCODE_RSHIFT];
    }
    return lshift || rshift;
}

bool is_ctrl_down() {
    int num_keys = 0;
    const Uint8 *key_state = SDL_GetKeyboardState(&num_keys);
    bool lctrl = false, rctrl = false;
    if (SDL_SCANCODE_LCTRL < num_keys) {
        lctrl = key_state[SDL_SCANCODE_LCTRL];
    }
    if (SDL_SCANCODE_RCTRL < num_keys) {
        rctrl = key_state[SDL_SCANCODE_RCTRL];
    }
    return lctrl || rctrl;
}

bool is_alt_down() {
    int num_keys = 0;
    const Uint8 *key_state = SDL_GetKeyboardState(&num_keys);
    bool lalt = false, ralt = false;
    if (SDL_SCANCODE_LALT < num_keys) {
        lalt = key_state[SDL_SCANCODE_LALT];
    }
    if (SDL_SCANCODE_RALT < num_keys) {
        ralt = key_state[SDL_SCANCODE_RALT];
    }
    return lalt || ralt;
}

Key_code get_key(const Keyboard_event &event) {
    return event.get_key();
}

float get_wheel_scroll(const Mouse_wheel_event &event) {
    return event.get_scrolled_y();
}

bool *get_key_state(int &num_keys) {
    static bool state[512] = {false};
    const Uint8 *key_state = SDL_GetKeyboardState(&num_keys);
    for (int i = 0; i < 512 && i < num_keys; ++i) {
        int keycode = static_cast<int>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(i)));
        if (keycode > 512) {
            continue;
        }
        state[keycode] = key_state[i];
    }
    return state;
}

int add_timer(int interval, uint32_t (*func)(uint32_t, void *), void *param) {
    return TIMER_MANAGER->add_timer(interval, func, param);
}

void remove_timer(int id) {
    TIMER_MANAGER->remove_timer(id);
}

}