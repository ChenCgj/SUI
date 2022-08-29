#ifndef BULLET_H
#define BULLET_H
#include "pixel_block.h"

class Bullet : public Pixel_block {
public:
    Bullet(Pixel_screen &screen);
    void do_action() override;
    using Pixel_block::move;
private:
};

#endif