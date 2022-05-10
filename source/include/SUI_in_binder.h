#ifndef SUI_IN_BINDER_H
#define SUI_IN_BINDER_H

#include <functional>
#include "SUI_property.h"
namespace sui {

class Binder_base {
public:
    Binder_base();
    virtual void update_value(const void *p) = 0;
    virtual ~Binder_base();
};

template<typename Origin, typename Target>
class Binder : public Binder_base {
public:
    Binder(Property<Target> &property, const std::function<Target (const Origin &)> func);
    void update_value(const void *pData) override;
    const Property<Target> &get_binder() const;
private:
    Property<Target> *property;
    std::function<Target (const Origin &)> func;
};

}
#endif