#include "SUI_utils.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include <utility>

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