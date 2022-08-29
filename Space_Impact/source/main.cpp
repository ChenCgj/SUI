#include <iostream>
#include "bullet.h"
#include "level.h"
#include "play_env.h"
#include "font.h"

using namespace std;
using namespace sui;

const int width = 1050;
const int height = 750;
const int width_resource = 70;
const int height_resource = 50;
Color background_color{112, 128, 112, 255};

Play_env env;
Level level;

uint32_t update_frame(uint32_t interval, void *arg);
void init_env(Play_env *penv);

int main(int argc, char *argv[])
{
    set_run_mode(Run_mode::wait);

    init_env(&env);
    level.load_env(&env);
    update_frame(0, &level);

    env.pWnd->show();
    return 0;
}

uint32_t update_frame(uint32_t interval, void *arg) {
    if (env.gameover) {
        static uint32_t value = 100;
        if (value == 100) {
            string end = "Gameover, your score: " + to_string(env.score);
            env.pWnd->add_node(new sui::Label(end, 35, 0, 0, env.pWnd->get_width(), 100));
            present_all();
        }
        return value--;
    }
    Level *level = static_cast<Level *>(arg);
    level->next_frame();
    present_all();
    return interval;
}

void init_env(Play_env *penv) {
    penv->bullet_delay = 0;
    Window *pWnd = new Window("Space Imapct", width, height);
    Graphic_board *board = new Graphic_board(0, 0, width, height);
    pWnd->add_node(board);

    Pixel_screen *screen = new Pixel_screen(width_resource, height_resource, background_color);
    screen->show_on_board(board);
    Player *player = new Player(*screen);
    player->set_type(Pixel_block::the_friend);

    penv->board = board;
    penv->pWnd = pWnd;
    penv->screen = screen;
    penv->player = player; 
    penv->score = 0;
    penv->gameover = false;

    pWnd->add_listener([=](Keyboard_event &event)->void {
        int num_keys;
        bool *state = get_key_state(num_keys);
        if (state[key_w]) {
            player->move(0, -1);
        }
        if (state[key_s]) {
            player->move(0, 1);
        }
        if (state[key_a]) {
            player->move(-1, 0);
        }
        if (state[key_d]) {
            player->move(1, 0);
        }
        if (state[key_j]) {
            if (penv->bullet_delay == 0) {
                Bullet *bullet = new Bullet(*screen);
                int posX, posY;
                penv->player->get_pos(posX, posY);
                posY += 5;
                posX += 12;
                bullet->set_pos(posX, posY);
                bullet->set_type(Pixel_block::the_friend_attack);
                penv->elements.push_back(bullet);
                penv->bullet_delay = 4;
            }
        }
    }, Key_event::down);
    int timerid = add_timer(100, update_frame, &level);
    board->register_clean([=](void *){remove_timer(timerid);}, nullptr);
    register_clean([=](void)->void {
        delete penv->screen;
        delete penv->player;
        for (auto p : penv->elements) {
            delete p;
        }
    });
}