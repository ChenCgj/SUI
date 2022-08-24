#include "SUI.h"
#include <ctime>

#define WIDTH 800
#define HEIGHT 600

using namespace std;
using namespace sui;

Color color{255, 255, 255, 255};
bool start = false;
int timer_id = -1;
int up_down = 1;
bool game_over = false;
std::list<Rect> rect_list;

Window *get_window();
Pane *get_main_pane();
Pane *get_start_pane();
Graphic_board *get_board();
void func_color_up();
void func_color_down();
void set_board();
void clean();

uint32_t timer_update(uint32_t interval, void *param);

int main(int argc, char *argv[]) {
    set_run_mode(poll);
    srand(time(nullptr));

    Window *pWindow = get_window();
    pWindow->add_listener([](Keyboard_event &e) {
        Key_code code = get_key(e);
        if (code == key_w) {
            func_color_up();
        } else if (code == key_s) {
            func_color_down();
        }
        present_all();
    }, Key_event::down);
    Pane *pmain_pane = get_main_pane();
    set_board();
    pWindow->add_node(pmain_pane);
    pWindow->show();
    timer_id = add_timer(50, timer_update, nullptr);
    register_clean(clean);
    return 0;
}

Window *get_window() {
    static Window *pWnd = new Window("runner", WIDTH, HEIGHT);
    return pWnd;
}

Pane *get_main_pane() {
    static bool has_exec = false;
    static Pane *ppane = nullptr;
    if (!has_exec) {
        has_exec = true;
        ppane = new Pane(0, 0, WIDTH, HEIGHT);
        Vertical_pane *vpane = new Vertical_pane(0, 0, WIDTH / 3, HEIGHT / 3);
        vpane->set_gap(50);
        ppane->add_node(vpane);
        ppane->add_content(*vpane, WIDTH / 3, 100);
        Button *pButton_start_game = new Button("START", 0, 0, 60, 100);
        Button *pButton_quit_game = new Button("QUIT", 0, 0, 60, 100);
        pButton_start_game->add_listener([=](){
            Window *pWnd = get_window();
            pWnd->remove_node(ppane);
            pWnd->add_node(get_start_pane());
            start = true;
        }, sui::Button::Button_event::up);
        pButton_quit_game->add_listener([](){
            quit();
        }, sui::Button::Button_event::up);
        Label *label = new Label("PLAY GAME", 50, 0, 0, 100, 100);
        vpane->add_node(label);
        vpane->add_node(pButton_start_game);
        vpane->add_node(pButton_quit_game);
        vpane->add_content(*label);
        vpane->add_content(*pButton_start_game);
        vpane->add_content(*pButton_quit_game);
    }
    return ppane;
}

Pane *get_start_pane() {
    static bool has_exec = false;
    static Pane *ppane = nullptr;
    if (!has_exec) {
        has_exec = true;
        ppane = new Pane(0, 0, WIDTH, HEIGHT);
        Vertical_pane *vpane = new Vertical_pane(0, 0, WIDTH / 10, HEIGHT / 5);
        vpane->set_gap(20);

        Button *pButton_return = new Button("<BACK", 0, 0, 60, 30);
        pButton_return->add_listener([=](){
            Window *pWnd = get_window();
            pWnd->remove_node(ppane);
            pWnd->add_node(get_main_pane());
            start = false;
        }, sui::Button::Button_event::up);
        vpane->add_node(pButton_return);
        vpane->add_content(*pButton_return);

        Button *pButton_up = new Button("up", 0, 0, WIDTH / 10, HEIGHT / 10);
        pButton_up->add_listener(func_color_up, Button::Button_event::up);

        Button *pButton_down = new Button("down", 0, 0, WIDTH / 10, HEIGHT / 10);
        pButton_down->add_listener(func_color_down, Button::Button_event::down);

        Graphic_board *board = get_board();
        ppane->add_node(board);
        ppane->add_content(*board, 0, 0);
        ppane->add_node(pButton_up);
        ppane->add_content(*pButton_up, WIDTH / 10, HEIGHT * 4 / 5);
        ppane->add_node(pButton_down);
        ppane->add_content(*pButton_down, WIDTH * 4 / 5, HEIGHT * 4 / 5);
        ppane->add_node(vpane);
        ppane->add_content(*vpane, 10, 0);
    }
    return ppane;
}

Graphic_board *get_board() {
    static Graphic_board *board = new Graphic_board(0, 0, WIDTH, HEIGHT);
    return board;
}

void set_board() {
    Graphic_board *board = get_board();
    board->set_draw_callback(std::function<void (Graphic_board_base *)>([](Graphic_board_base *pb){
        pb->set_draw_color(color, true);
        pb->clear(true);
        pb->set_draw_color(Color{200, 0, 0, 255}, true);
        for (auto iter = rect_list.begin(); iter != rect_list.end(); ++iter) {
            pb->fill_rect(*iter, true);
        }
        pb->set_draw_color(Color{0, 0, 0, 255}, true);
        pb->draw_line(0, HEIGHT / 2, WIDTH, HEIGHT / 2, true);
        pb->draw_line(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2 + 50 * up_down, true);
    }));
}

void func_color_up() {
    // int red = color.red + 10;
    // color.red = red < 255 ? red : 255;
    up_down = -1;
    get_board()->set_redraw_flag(true);
}

void func_color_down() {
    // int red = color.red - 10;
    // color.red = red < 0 ? 0 : red;
    up_down = 1;
    get_board()->set_redraw_flag(true);
}

uint32_t timer_update(uint32_t interval, void *param) {
    if (!start) {
        return interval;
    }

    int random = rand();
    if (random % 17 == 0) {
        int dir = random % 6 == 0 ? 1 : -1;
        if (rect_list.empty() || rect_list.back().p2.x < WIDTH - 40) {
            Rect rect{WIDTH, static_cast<double>(HEIGHT / 2.0 + 40 * dir), WIDTH + 30, static_cast<double>(HEIGHT / 2.0)};
            rect_list.push_back(rect);
        }
    }

    for (auto iter = rect_list.begin(); iter != rect_list.end();) {
        if (iter->p1.x < WIDTH / 2.0 && iter->p2.x > WIDTH / 2.0 && (iter->p1.y - HEIGHT / 2.0) * up_down > 0) {
            game_over = true;
        }
        iter->p1.x -= 10;
        iter->p2.x -= 10;
        if (iter->p2.x < 0) {
            iter = rect_list.erase(iter);
        } else {
            ++iter;
        }
    }

    get_board()->set_redraw_flag(true);
    // present(get_window());
    if (game_over) {
        return 0;
    }
    return interval;
}

void clean() {
    remove_timer(timer_id);
}