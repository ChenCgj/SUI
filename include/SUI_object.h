/**
* @file SUI_object.h
* @brief contains the definition of Object
*/

#ifndef SUI_OBJECT_H
#define SUI_OBJECT_H

#include "SUI_canvas.h"
#include <list>
#include <string>

namespace sui {
class Window;

/**
* @class Object
* @brief this is the base class of all element will on the object tree
*        every node in the root/trash_root will be delete auto
* @warning should not create this object directly, it will create a another root!
*          you can define other class dervied from this class and rember to add it to the trash_root initial
* @warning @bug it's unsafe to call get_node_list() in multi-thread program
*/

class Object {
public:
    static Object *get_trash_root();
    static Object *get_root();
    bool add_node(Object * pObject);
    bool add_nodes(const std::list<Object *> pObjects);
    bool remove_node(Object *pObject);
    bool remove_nodes(const std::list<Object *> pObjects);
    Object *get_parent();
    // this function is insafe in multi-program
    // when you call this, you should confirm that you will not make the child deletable
    std::list<Object *> get_node_list();
    bool destroy(bool flag);
    virtual std::string get_name();
    virtual ~Object();
protected:
    std::string object_name;
    Object();
    Object *set_parent(Object *parent);
private:
    Object *parent;
    std::list<Object *> object_list;
    bool can_delete;
    // a tool function to move a object
    void add_node_from(Object *src, Object *child);
    int ID;
    friend int clean_trash(void *data);
};

} // end of sui
#endif