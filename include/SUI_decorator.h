/**
* @file SUI_decorator.h
* @brief contain definition of class Decorator
*/

#ifndef SUI_DECORATOR_H
#define SUI_DECORATOR_H

#include <memory>
#include <stdint.h>

namespace sui {

/**
* @class Decorator
* @brief decorate some element which derived from this class
*/

class Decorator {
public:
    Decorator();
    virtual ~Decorator();
    void set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    void get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const;
    void set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    void get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const;
    void set_border_radius(unsigned radius);
    int get_border_radius() const;
private:
    struct Decorator_data;
    std::unique_ptr<Decorator_data> pData;
};
}
#endif