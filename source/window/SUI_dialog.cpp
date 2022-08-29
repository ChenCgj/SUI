#include "SUI_in_main.h"
#include "SUI_in_managers.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_debug.h"

#include "SUI_pane_vertical.h"
#include "SUI_pane_horizontal.h"
#include "SUI_button.h"
#include "SUI_graphic_board.h"
#include "SUI_dialog.h"
namespace sui {

static void add_dialog_element(Dialog &dialog, const std::string &message);

Dialog::Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, int posX, int posY, Window_flag flag)
    : Geometry{0, 0, width, height}, Window_base{title, width, height, posX, posY, flag} {
    object_name = "dialog";
    if (parent) {
        if (SDL_SetWindowModalFor(WINDOW_MANAGER->get_sdl_window(this), WINDOW_MANAGER->get_sdl_window(parent)) < 0) {
            ERR(<< "sdl: " << SDL_GetError());
        }
    }
    add_dialog_element(*this, message);
}

Dialog::Dialog(Window_base *parent, const std::string &message, const std::string &title, int width, int height, Window_flag flag)
    : Geometry{0, 0, width, height}, Window_base{title, width, height, flag} {
    object_name = "dialog";
    if (parent) {
        if (SDL_SetWindowModalFor(WINDOW_MANAGER->get_sdl_window(this), WINDOW_MANAGER->get_sdl_window(parent)) < 0) {
            ERR(<< "sdl: " << SDL_GetError());
        }
    }
    add_dialog_element(*this, message);
}

static void add_dialog_element(Dialog &dialog, const std::string &message) {
    Button *yes_bt = new Button("Yes");
    Button *no_bt = new Button("No");
    Horizontal_pane *hp = new Horizontal_pane(0, 0, 200, 50);
    hp->set_gap(20);
    hp->add_node(yes_bt);
    hp->add_content(*yes_bt);
    hp->add_node(no_bt);
    hp->add_content(*no_bt);
    Graphic_board *board = new Graphic_board(0, 0, 200, 80);
    Rect rect = {0, 0, 200, 80};
    board->draw_text(rect, message, "consola.ttf", {0, 0, 0, 255}, 30);
    Vertical_pane *vp = new Vertical_pane(0, 0, 200, 150);
    vp->add_node(board);
    vp->add_content(*board);
    vp->add_node(hp);
    vp->add_content(*hp);
    dialog.add_node(vp);
}

Dialog::~Dialog() {
    prepare_destroy();
}
}