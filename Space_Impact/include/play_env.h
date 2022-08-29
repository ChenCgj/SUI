#ifndef PLAY_ENV_H
#define PLAY_ENV_H
#include "pixel_screen.h"
#include "player.h"

class Play_env {
public:
    sui::Window *pWnd;
    Pixel_screen *screen;
    sui::Graphic_board *board;
    Player *player;
    std::vector<Pixel_block *> elements;
    int bullet_delay;
    unsigned score;
    bool gameover;
};

#endif