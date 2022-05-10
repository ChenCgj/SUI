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
    ~Decorator();
    virtual void set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    virtual void get_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const;
    virtual void set_background_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    virtual void get_background_color(uint8_t &red, uint8_t &green, uint8_t &blue, uint8_t &alpha) const;
private:
    struct Decorator_data;
    std::unique_ptr<Decorator_data> pData;
};
}
#endif