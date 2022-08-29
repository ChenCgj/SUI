#include <iostream>
#include "level.h"
#include "pixel_block.h"
#include "pixel_screen.h"
#include "play_env.h"
#include "font.h"

extern sui::Color background_color;

Level::Level() : level_id(1), env(nullptr), background(nullptr), background_posX(0), background_posY(0) {}

void Level::load_env(Play_env *env) {
    this->env = env;
    load_background();
}

void Level::load_background() {
    if (background) {
        delete background;
    }
    if (level_id == 1) {
        env->player->set_y_limit(6, env->screen->get_height() - 1);
        return;
    }
    // TODO
}

void Level::draw_statu() {
    if (level_id == 1) {

    }
}

void Level::generate_enemy() {
    static int count = 10;
    if (level_id == 1) {
        if (count != 0) {
            count--;
            return;
        }
        Pixel_block *enemy = new Pixel_block;
        enemy->load_data("./resource/enemy1.dat");
        int posX = env->screen->get_width() - 5;
        int posY = rand() % (env->screen->get_height() - 10) + 5;
        enemy->set_velocity(-1, 0);
        enemy->set_pos(posX, posY);
        enemy->set_type(Pixel_block::the_enemy);
        env->elements.push_back(enemy);
        count = 10;
    }
}

void Level::next_frame() {
    Pixel_screen *screen = env->screen;
    screen->clean(background_color);
    int posX, posY;
    env->player->get_pos(posX, posY);
    env->player->draw_on_other(screen, posX, posY);
    if (background) {
        background->draw_on_other(screen, background_posX, background_posY);
    }
    for (auto iter = env->elements.begin(); iter != env->elements.end();) {
        Pixel_block *ele = dynamic_cast<Pixel_block *>(*iter);
        if (ele == nullptr) {
            continue;
        }
        ele->do_action();
        int posX, posY;
        ele->get_pos(posX, posY);
        ele->draw_on_other(screen, posX, posY);
        if (ele->is_out_of_screen()) {
            iter = env->elements.erase(iter);
        } else {
            if (detect_collision(*env->player, **iter)) {
                if ((*iter)->get_type() == Pixel_block::the_enemy || (*iter)->get_type() == Pixel_block::the_enemy_attack) {
                    int health = env->player->get_health();
                    env->player->set_health(--health);
                    if (health == 0) {
                        env->gameover = true;
                        return;
                    }
                    (*iter)->set_out_of_screen(true);
                }
            }
            for (auto e_iter = env->elements.begin(); e_iter != env->elements.end(); ++e_iter) {
                for (auto e_iter2 = e_iter + 1; e_iter2 != env->elements.end(); ++e_iter2) {
                    if ((*e_iter)->is_out_of_screen() || (*e_iter2)->is_out_of_screen()) {
                        continue;
                    }
                    int t1 = -1, t2 = -1;
                    if ((*e_iter)->get_type() == Pixel_block::the_friend_attack) {
                        t1 = 0;
                    } else if ((*e_iter)->get_type() == Pixel_block::the_enemy_attack || (*e_iter)->get_type() == Pixel_block::the_enemy) {
                        t1 = 1;
                    }
                    if ((*e_iter2)->get_type() == Pixel_block::the_friend_attack) {
                        t2 = 0;
                    } else if ((*e_iter2)->get_type() == Pixel_block::the_enemy_attack || (*e_iter2)->get_type() == Pixel_block::the_enemy) {
                        t2 = 1;
                    }
                    if (t1 != t2) {
                        if (detect_collision(**e_iter, **e_iter2)) {
                            env->score += 10;
                            (*e_iter)->set_out_of_screen(true);
                            (*e_iter2)->set_out_of_screen(true);
                        }
                    }
                }
            }
            ++iter;
        }
    }
    if (env->bullet_delay > 0) {
        env->bullet_delay--;
    }

    if (level_id == 1) {
        posX = 1, posY = 1;
        for (int i = 0; i < env->player->get_health(); ++i) {
            Font::instance()->print_heart(screen, posX, posY);
            posX += 6;
        }

        posX = 23;
        Font::instance()->print_big_bullet(screen, posX, posY, 0);
        posX = 29;
        Font::instance()->print_num(screen, posX, posY, env->player->get_big_bullet() / 10);
        Font::instance()->print_num(screen, posX + 4, posY, env->player->get_big_bullet());
        int score = env->score;
        posX = 50;
        int mod = 1e4;
        for (int i = 0; i < 5; ++i) {
            Font::instance()->print_num(screen, posX, posY, (score / mod) % 10);
            mod /= 10;
            posX += 4;
        }

    }
    generate_enemy();
    env->board->set_redraw_flag(true);
}

