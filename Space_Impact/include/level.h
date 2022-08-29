#ifndef LEVEL_H
#define LEVEL_H

#include "pixel_block.h"
class Play_env;
class Level {
public:
    Level();
    void load_env(Play_env *env);
    void next_frame();
private:
    int level_id;
    Play_env *env;
    Pixel_block *background;
    int background_posX;
    int background_posY;
    void load_background();
    void generate_enemy();
    void draw_statu();
};
#endif