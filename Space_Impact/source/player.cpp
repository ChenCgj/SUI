#include "player.h"

Player::Player(Pixel_screen &screen) : Pixel_block(screen), safe_mode(false), big_bullet(0) {
    load_data("./resource/player.dat");
    set_pos(30, 30);
    set_health(3);
}

void Player::move(int dx, int dy) {
    int posX, posY;
    get_pos(posX, posY);
    int screen_x, screen_y;
    get_screen_size(screen_x, screen_y);
    if (posX + dx + 12 > screen_x || posX + dx + 2 < 0 || posY + dy + 2 < 0 || posY + dy + 9 > screen_y) {
        return;
    }
    int top, bottom;
    get_y_limit(top, bottom);
    if ((top < bottom) && (posY + dy + 2 < top || posY + dy + 9 > bottom + 1)) {
        return;
    }
    Pixel_block::move(dx, dy);
}

int Player::get_big_bullet() const {
    return big_bullet;
}

void Player::set_big_bullet(int bigbullet) {
    big_bullet = bigbullet;
}