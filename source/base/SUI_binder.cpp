#include "SUI_in_binder.h"

namespace sui {

Binder_base::Binder_base() = default;
Binder_base::~Binder_base() {}
/**
* @todo make the binder can deal with multi bind
*/
template<typename Origin, typename Target>
Binder<Origin, Target>::Binder(Property<Target> &property, const std::function<Target (const Origin &)> func)
    : property{&property}, func{func} {}

template<typename Origin, typename Target>
void Binder<Origin, Target>::update_value(const void *pData) {
    const Origin &value = *static_cast<const Origin *>(pData);
    property->set_value(func(value));
}

template<typename Origin, typename Target>
const Property<Target> &Binder<Origin, Target>::get_binder() const {
    return *property;
}

template class Binder<int, int>;
}