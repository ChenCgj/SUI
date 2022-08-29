#include "SUI_in_main.h"
#include "SUI_in_debug.h"
#include "SUI_in_managers.h"
#include "SUI_object.h"

namespace sui {

// it's unsafe to use this global static variable to intial others static object
// this thread is to clean the trash_root, no use now
// SDL_Thread *clean_thread = nullptr;

int clean_trash(void *data);
/**
* @todo this function should be static, but friend function can't set to static, avoid friend function!
* what's more is make this function run in other thread to increase the performence of the program
*/
// very very terrible design to use object_list
/*static */int clean_trash(void *data) {
    // bool flag = true;
    // while (flag) {
    //     for (auto iter = TRASH_ROOT->object_list.begin(); iter != TRASH_ROOT->object_list.end(); ++iter) {
    //         if (*iter == TRASH_ROOT) {
    //             flag = false;
    //             break;
    //         } else if ((*iter)->set_destroy(false) == true) {
    //             delete *iter;
    //             iter = TRASH_ROOT->object_list.erase(iter);
    //         }
    //     }
    // }
    for (auto iter = TRASH_ROOT->object_list.begin(); iter != TRASH_ROOT->object_list.end();) {
        if ((*iter)->set_destroy(false) == true) {
            delete *iter;
            iter = TRASH_ROOT->object_list.erase(iter);
        } else {
            ++iter;
        }
    }
    return 0;
}

// use to debug conveniently
std::string Object::get_name() {
    std::string name = object_name;
    name += "_object_id:" + std::to_string(ID);
    return name;
}

Object::Object() : object_name{"Object"}, ID{-1}, parent{nullptr}, object_list{}, can_delete{false} {
    static int id_count = 0;
    ID = id_count++;

    run_clean = true;
    clean_func = nullptr;
    clean_arg = nullptr;
}

void Object::prepare_destroy() {
    if (run_clean && clean_func) {
        clean_func(clean_arg);
        run_clean = false;
        clean_arg = nullptr;
    }
}

void Object::register_clean(std::function<void (void *)> func, void *arg) {
    clean_func = func;
    clean_arg = arg;
}

Object *Object::root_instance() {
    // sould use static instead of new, otherwise you will not know when to destroy it
    // static Object root;
    static Object *root = new Object();
    static bool first = true;
    if (first) {
        first = false;
        root->object_name += "_init_root";
    }
    return root;
}

Object *Object::trash_root_instance() {
    static Object *trash_root = new Object();
    // static Object trash_root;
    static bool first = true;
    if (first) {
        // clean_thread = SDL_CreateThread(clean_trash, "clean trash", nullptr);
        trash_root->object_name += "_trash_root";
        first = false;
    }
    return trash_root;
}

bool Object::add_node(Object *pObject) {
    // when a object is on trash_root or on null it can be add to a new parent
    if (pObject->get_parent() != TRASH_ROOT && pObject->get_parent() != nullptr) {
        return false;
    } else {
        add_node_from(pObject->get_parent(), pObject);
    }
    return true;
}

bool Object::add_nodes(const std::list<Object *> pObjects) {
    for (auto p : pObjects) {
        if (p->get_parent() != TRASH_ROOT && p->get_parent() != nullptr) {
            return false;
        }
    }
    for (auto p : pObjects) {
        add_node_from(p->get_parent(), p);
    }
    return true;
}

bool Object::remove_node(Object *pObject) {
    // it should not be removed from the trash_root so that every object we provide to the user can be delete
    // otherwise may be lead to memory leak
    if (pObject->get_parent() == nullptr || pObject->get_parent() == TRASH_ROOT) {
        return false;
    } else {
        TRASH_ROOT->add_node_from(this, pObject);
    }
    return true;
}

bool Object::remove_nodes(const std::list<Object *> pObjects) {
    for (auto p : pObjects) {
        if (p->get_parent() == nullptr || p->get_parent() == TRASH_ROOT) {
            return false;
        }
    }
    for (auto p : pObjects) {
        TRASH_ROOT->add_node_from(this, p);
    }
    return true;
}

Object *Object::set_parent(Object *parent) {
    Object *temp = this->parent;
    this->parent = parent;
    return temp;
}

Object *Object::get_parent() {
    return parent;
}

// flag a object can be delete on trash_root
// warning : you should not use a object when it was on trash_root and be flag deleteale
bool Object::set_destroy(bool flag) {
    if (flag && get_parent() == TRASH_ROOT) {
        can_delete = true;
    }
    return can_delete;
}

Object::~Object() {
    prepare_destroy();
    DBG(<< "delete " << get_name() << "'s " << "children");
    // delete all children
    for (auto p : object_list) {
        delete p;
    }
    DBG(<< get_name() << " delete ok");
}

void Object::add_node_from(Object *src, Object *child) {
    // lock the object tree
    // SDL_LockMutex(lock_object_tree);
    if (src != nullptr) {
        src->object_list.remove(child);
    }
    object_list.push_back(child);
    child->set_parent(this);
    // SDL_UnlockMutex(lock_object_tree);
}

// unsafe function
// you should not make any object to be deletable on TRASH_ROOT while using the reture value
// unless you can confirm that the object you set is not in the list returned
std::list<Object *> Object::get_node_list() {
    std::list<Object *> ret;
    // SDL_LockMutex(lock_object_tree);
    ret = object_list;
    // SDL_UnlockMutex(lock_object_tree);
    return ret;
}
}