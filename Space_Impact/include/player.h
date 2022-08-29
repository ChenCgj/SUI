#ifndef PLAYER_H
#define PLAYER_H
#include "pixel_block.h"

class Player : public Pixel_block {
public:
    Player(Pixel_screen &screen);
    void move(int dx, int dy) override;
    int get_big_bullet() const;
    void set_big_bullet(int bigbullet);
private:
    bool safe_mode;
    int big_bullet;
};

#endif