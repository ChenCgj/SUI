#ifndef PIXEL_BLOCK_H
#define PIXEL_BLOCK_H

#include <vector>
#include <string>
#include "pixel_element.h"
#include "pixel_screen.h"

class Pixel_block : public Pixel_element {
public:
    enum Type {
        the_friend, the_friend_attack, the_enemy, the_enemy_attack, unknow
    };
    Pixel_block(Pixel_screen &screen);
    Pixel_block();
    virtual void load_data(const std::string &init_file);
    void set_pos(int posX, int posY);
    void get_pos(int &posX, int &posY) const;
    void get_velocity(int &vx, int &vy) const;
    void set_velocity(int vx, int vy);
    virtual void move(int dx, int dy);
    void get_screen_size(int &x, int &y) const;
    void set_out_of_screen(bool flag);
    bool is_out_of_screen() const;
    void set_type(Type type);
    friend bool detect_collision(const Pixel_block &pb1, const Pixel_block &pb2);
    Type get_type() const;
    virtual void do_action();
    void get_y_limit(int &top, int &bottom) const;
    void set_y_limit(int top, int bottom);
    int get_health() const;
    void set_health(int health);
    virtual ~Pixel_block();
private:
    int mposX, mposY;
    int mhealth;
    int mvelocity_x, mvelocity_y;
    Pixel_screen *mscreen;
    bool mout_of_screen;
    Type mtype;
    std::pair<int, int> mlimit_posY;
};
bool detect_collision(const Pixel_block &pb1, const Pixel_block &pb2);

#endif