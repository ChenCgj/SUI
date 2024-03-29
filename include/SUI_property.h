/**
* @file SUI_property.h
* @brief contains definition of class Property
*/

#ifndef SUI_PROPERTY_H
#define SUI_PROPERTY_H

#include <list>
#include <functional>
namespace sui {

class Binder_base;

/**
* @class Property
* @brief provide binder, so that it can watch other property to update is self
* @todo why not bind to as more as possible properies?
*/
template<typename T>
class Property {
public:
    Property(const T &value);
    Property();
    Property(const Property &) = delete;
    Property &operator=(const Property &) = delete;
    ~Property();
    const T &get_value() const;
    void set_value(const T &value);
    void set_listener(const std::function<void (const T &value, void *)> &func, void *args);
    // bind to others, when other porperty change, this property can update
    template<typename Origin>
    void bind(Property<Origin> &property, std::function<T (const Origin &)>);
    template<typename Origin>
    void unbind(Property<Origin> &property);
    // bind to self, when the value of this object is change, it will update these property
    template<typename Target>
    void add_binded(Property<Target> &property, std::function<Target (const T &)> func);
    template<typename Target>
    void remove_binded(Property<Target> &property);
private:
    T value;
    std::function<void (const T &value, void *)> on_change;
    void *args;
    std::list<Binder_base *> binders;
};

}
#endif