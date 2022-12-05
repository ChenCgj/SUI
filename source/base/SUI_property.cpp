#include "SUI_property.h"
#include "SUI_in_binder.h"

namespace sui {
extern template class Binder<int, int>;

template<typename T>
Property<T>::Property(const T &value) : value{value}, args{nullptr} {}
template<typename T>
Property<T>::Property() : value{}, args{nullptr} {}

template<typename T>
const T &Property<T>::get_value() const {
    return value;
}

template<typename T>
void Property<T>::set_value(const T &value) {
    this->value = value;
    if (on_change) {
        on_change(value, args);
    }
    // update all
    for (auto p : binders) {
        p->update_value(&value);
    }
}

template <typename T>
void Property<T>::set_listener(const std::function<void (const T &value, void *)> &func, void *args) {
    on_change = func;
    this->args = args;
}

template<typename T>
template<typename Target>
void Property<T>::add_binded(Property<Target> &property, std::function<Target (const T &)> func) {
    binders.push_back(new Binder<T, Target>(property, func));
    property.set_value(func(get_value()));
}

template<typename T>
template<typename Target>
void Property<T>::remove_binded(Property<Target> &property) {
    Binder_base *pbb = nullptr;
    for (auto iter = binders.begin(); iter != binders.end();) {
        if (&dynamic_cast<Binder<T, Target> *>(*iter)->get_binder() == &property) {
            pbb = *iter;
            iter = binders.erase(iter);
            delete pbb;
        } else {
            ++iter;
        }
    }
}

template<typename T>
template<typename Origin>
void Property<T>::bind(Property<Origin> &property, std::function<T (const Origin &)> func) {
    property.add_binded(*this, func);
}

template<typename T>
template<typename Origin>
void Property<T>::unbind(Property<Origin> &property) {
    property.remove_binded(*this);
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
template void Property<int>::remove_binded<>(Property<int> &property);
template void Property<int>::unbind<>(Property<int> &property);
}