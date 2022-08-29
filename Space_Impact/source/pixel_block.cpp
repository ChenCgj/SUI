#include <fstream>
#include "pixel_block.h"

Pixel_block::Pixel_block(Pixel_screen &screen) : Pixel_element(), mposX(0), mposY(0), mhealth(1),
    mvelocity_x(0), mvelocity_y(0), mscreen(&screen), mout_of_screen(false), mtype(unknow), mlimit_posY(std::make_pair(0, -1)) {}

Pixel_block::Pixel_block() : Pixel_element(), mposX(0), mposY(0), mhealth(1), mvelocity_x(0), mvelocity_y(0),
    mscreen(nullptr), mout_of_screen(false), mtype(unknow) {}

Pixel_block::~Pixel_block() {}

void Pixel_block::load_data(const std::string &init_file) {
    std::ifstream input(init_file);
    input >> mwidth;
    input >> mheight;
    mpixels = std::vector<std::vector<sui::Color>>(mheight, std::vector<sui::Color>(mwidth, {0, 0, 0, 0}));
    for (int y = 0; y < mheight; ++y) {
        for (int x = 0; x < mwidth; ++x) {
            int c;
            input >> c;
            if (c == 1) {
                set_pixel(x, y, Pixel_element::black);
            }
        }
    }
    input.close();
}

void Pixel_block::set_pos(int posX, int posY) {
    mposX = posX;
    mposY = posY;
}

void Pixel_block::get_pos(int &posX, int &posY) const {
    posX = mposX;
    posY = mposY;
}

void Pixel_block::move(int dx, int dy) {
    mposX = mposX + dx;
    mposY = mposY + dy;
}

void Pixel_block::set_velocity(int vx, int vy) {
    mvelocity_x = vx;
    mvelocity_y = vy;
}

void Pixel_block::get_velocity(int &vx, int &vy) const {
    vx = mvelocity_x;
    vy = mvelocity_y;
}

void Pixel_block::do_action() {
    move(mvelocity_x, mvelocity_y);
}

void Pixel_block::get_screen_size(int &x, int &y) const {
    x = mscreen->get_width();
    y = mscreen->get_height();
}

void Pixel_block::set_out_of_screen(bool flag) {
    mout_of_screen = flag;
}

bool Pixel_block::is_out_of_screen() const {
    return mout_of_screen;
}

void Pixel_block::set_type(Type type) {
    mtype = type;
}

Pixel_block::Type Pixel_block::get_type() const {
    return mtype;
}

void Pixel_block::set_y_limit(int top, int bottom) {
    mlimit_posY = std::make_pair(top, bottom);
}

void Pixel_block::get_y_limit(int &top, int &bottom) const {
    top = mlimit_posY.first;
    bottom = mlimit_posY.second;
}

int Pixel_block::get_health() const {
    return mhealth;
}

void Pixel_block::set_health(int health) {
    mhealth = health;
}

bool detect_collision(const Pixel_block &pb1, const Pixel_block &pb2) {
    sui::Rect r1(pb1.mposX, pb1.mposY, pb1.mposX + pb1.get_width(), pb1.mposY + pb1.get_height());
    sui::Rect r2(pb2.mposX, pb2.mposY, pb2.mposX + pb2.get_width(), pb2.mposY + pb2.get_height());
    sui::Rect inter = sui::get_intersection(r1, r2);
    // double to int...
    int inter_w = (inter.get_width());
    int inter_h = (inter.get_height());
    if (inter_w <= 0 || inter_h <= 0) {
        return 0;
    }
    bool result = false;
    int start_x = inter.p1.x;
    int start_y = inter.p1.y;
    for (int y = 0; y < inter_h && !result; ++y) {
        for (int x = 0; x < inter_w && !result; ++x) {
            int p1x = start_x + x - pb1.mposX;
            int p1y = start_y + y - pb1.mposY;
            int p2x = start_x + x - pb2.mposX;
            int p2y = start_y + y - pb2.mposY;
            const sui::Color *c1, *c2;
            c1 = pb1.get_pixel(p1x, p1y);
            c2 = pb2.get_pixel(p2x, p2y);
            if (c1 != nullptr && c2 != nullptr && c1->alpha && c2->alpha) {
                result = true;
            }
        }
    }
    return result;
}