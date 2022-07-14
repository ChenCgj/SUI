#ifndef SUI_MAIN_H
#define SUI_MAIN_H
#ifndef main
#define main SUI_main
#endif
namespace sui {
enum Theme_index {
    none_theme,
    current_theme,
    default_theme,
    custom_theme
};

enum Element_status {
    None,
    normal,
    button_normal,
    button_press,
    button_hover
};

enum Key_code {
    key_unknow = 0,
    key_return = '\r',
    key_escape = '\b',
    key_tab = '\t',
    key_space = ' ',
    key_exclaim = '!',
    key_quotedbl = '"',
    key_hash = '#',
    key_percent = '%',
    key_dollar = '$',
    key_ampersand = '&',
    key_quote = '\'',
    key_left_paren = '(',
    key_right_paren = ')',
    key_asterisk = '*',
    key_plus = '+',
    key_comma = ',',
    key_minus = '-',
    key_period = '.',
    key_slash = '/',
    key_0 = '0',
    key_1 = '1',
    key_2 = '2',
    key_3 = '3',
    key_4 = '4',
    key_5 = '5',
    key_6 = '6',
    key_7 = '7',
    key_8 = '8',
    key_9 = '9',
    key_colon = ':',
    key_semicolon = ';',
    key_less = '<',
    key_equals = '=',
    key_greater = '>',
    key_question = '?',
    key_at = '@',

    key_left_bracket = '[',
    key_backslash = '\\',
    key_right_bracket = ']',
    key_caret = '^',
    key_underscore = '_',
    key_backquote = '`',

    key_a = 'a',
    key_b = 'b',
    key_c = 'c',
    key_d = 'd',
    key_e = 'e',
    key_f = 'f',
    key_g = 'g',
    key_h = 'h',
    key_i = 'i',
    key_j = 'j',
    key_k = 'k',
    key_l = 'l',
    key_m = 'm',
    key_n = 'n',
    key_o = 'o',
    key_p = 'p',
    key_q = 'q',
    key_r = 'r',
    key_s = 's',
    key_t = 't',
    key_u = 'u',
    key_v = 'v',
    key_w = 'w',
    key_x = 'x',
    key_y = 'y',
    key_z = 'z',
};

enum Mouse_button {
    mouse_left_button,
    mouse_middle_button,
    mouse_right_button,
    mouse_unknow_button
};

void present_all();
void set_theme(Theme_index index);
}
#endif