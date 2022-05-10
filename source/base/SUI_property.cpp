#include <functional>
#include "SUI_property.h"
#include "SUI_in_binder.h"

namespace sui {
extern template class Binder<int, int>;

template<typename T>
Property<T>::Property(const T &value) : value{value} {}
template<typename T>
Property<T>::Property() : value{} {}

template<typename T>
const T &Property<T>::get_value() const {
    return value;
}

template<typename T>
void Property<T>::set_value(const T &value) {
    this->value = value;
    // update all
    for (auto p : binders) {
        p->update_value(&value);
    }
}

template<typename T>
template<typename Target>
void Property<T>::add_binded(Property<Target> &property, std::function<Target (const T &)> func) {
    binders.push_back(new Binder<T, Target>(property, func));
    property.set_value(func(get_value()));
}

template<typename T>
template<typename Origin>
void Property<T>::bind(Property<Origin> &property, std::function<T (const Origin &)> func) {
    property.add_binded(*this, func);
}

template<typename T>
Property<T>::~Property() {
    // clean all
    for (auto p : binders) {
        delete(p);
    }
}
// build template
template class Property<int>;
template void Property<int>::add_binded<>(Property<int> &property, std::function<int (const int &)> func);
template void Property<int>::bind<>(Property<int> &property, std::function<int (const int &)> func);
}