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
void present_all();
void set_theme(Theme_index index);
}
#endif