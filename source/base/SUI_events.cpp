#include "SDL_events.h"
#include "SUI_in_events.h"

namespace sui {

Event::Event(const SDL_Event &e) : event{e} {}

Keyboard_event::Keyboard_event(const SDL_Event &e) : Event{e} {}

Text_editint_event::Text_editint_event(const SDL_Event &e) : Event{e} {}

Text_input_event::Text_input_event(const SDL_Event &e) : Event{e} {}

Mouse_motion_event::Mouse_motion_event(const SDL_Event &e) : Event{e} {}

Mouse_button_event::Mouse_button_event(const SDL_Event &e) : Event{e} {}

Mouse_wheel_event::Mouse_wheel_event(const SDL_Event &e) : Event{e} {}

}