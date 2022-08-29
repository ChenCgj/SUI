#include "bullet.h"

Bullet::Bullet(Pixel_screen &screen) : Pixel_block(screen) {
    load_data("./resource/bullet.dat");
    set_pos(0, 0);
    set_velocity(1, 0);
}

void Bullet::do_action() {
    int vx, vy;
    get_velocity(vx, vy);
    int posX, posY;
    get_pos(posX, posY);
    int sx, sy;
    get_screen_size(sx, sy);
    if (posX + vx >= sx - 1 || posY + vy >= sy - 1 || posX + vx <= -4 || posY + vy <= -1) {
        set_out_of_screen(true);
    }
    move(vx, vy);
}