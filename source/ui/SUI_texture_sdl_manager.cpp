#include "SUI_in_main.h"
#include "SUI_in_debug.h"
#include "SUI_in_managers.h"
#include "SUI_in_window_manager.h"
#include "SUI_in_texture_sdl_manager.h"

namespace sui {

long long Texture_sdl_manager::curr_id = 1;

Texture_sdl_manager::Texture_sdl_manager() = default;

long long Texture_sdl_manager::alloc_texture_id() {
    Texture_sdl_manager_unit *unit = &texture_map[curr_id];
    unit->texture = nullptr;
    unit->prenderer = nullptr;
    // unit->auto_update = false;
    unit->static_access = false;
    // unit->update_func = nullptr;
    unit->id = curr_id;
    unit->valid = false;
    long long retval = curr_id++;
    while (texture_map.find(curr_id) != texture_map.end()) {
        ++curr_id;
    }
    return retval;
}

Texture_sdl_manager::~Texture_sdl_manager() {
    for (auto iter = texture_map.begin(); iter != texture_map.end(); ++iter) {
        if (iter->second.texture) {
            SDL_DestroyTexture(iter->second.texture);
            iter->second.texture = nullptr;
            WINDOW_MANAGER->update_texture_count(iter->second.prenderer, -1);
        }
        ERR(<< "not free all the texture!");
    }
    DBG(<< "texture_sdl_manager delete ok");
}

Texture_sdl_manager *Texture_sdl_manager::instance() {
    static Texture_sdl_manager *manager = new Texture_sdl_manager();
    return manager;
}

bool Texture_sdl_manager::free_texture_id(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        return false;
    } else {
        Texture_sdl_manager_unit *unit = &texture_map[id];
        if (unit->texture) {
            SDL_DestroyTexture(unit->texture);
            unit->texture = nullptr;
            WINDOW_MANAGER->update_texture_count(unit->prenderer, -1);
        }
        texture_map.erase(id);
        return true;
    }
}

SDL_Texture *Texture_sdl_manager::get_texture(long long id) {
    if (texture_map.find(id) != texture_map.end()) {
        return texture_map[id].texture;
    } else {
        ERR(<< "use a invalid sdl_texture id");
        return nullptr;
    }
}

SDL_Renderer *Texture_sdl_manager::get_renderer(long long id) {
    if (texture_map.find(id) != texture_map.end()) {
        return texture_map[id].prenderer;
    } else {
        ERR(<< "use a invalid sdl_texture id");
        return nullptr;
    }
}

SDL_Texture *Texture_sdl_manager::set_texture(long long id, SDL_Renderer *prenderer, Uint32 format, int access, int w, int h, SDL_BlendMode blend_mode) {
    if (texture_map.find(id) != texture_map.end()) {
        Texture_sdl_manager_unit *unit = &texture_map[id];
        if (unit->texture) {
            SDL_DestroyTexture(unit->texture);
            DBG(<< "texture destroy: " << (void*)unit->texture);
            unit->texture = nullptr;
            WINDOW_MANAGER->update_texture_count(unit->prenderer, -1);
            unit->valid = false;
        }
        unit->prenderer = prenderer;
        if (prenderer == nullptr) {
            unit->texture = nullptr;
            unit->valid = false;
            return nullptr;
        }
        unit->texture = SDL_CreateTexture(prenderer, format, access, w, h);
        if (!unit->texture) {
            ERR(<< "couldn't create the texture. SDL: " << SDL_GetError());
        } else {
            unit->static_access = access == SDL_TEXTUREACCESS_STATIC;
            SDL_SetTextureBlendMode(unit->texture, blend_mode);
            unit->valid = true;
            WINDOW_MANAGER->update_texture_count(unit->prenderer, 1);
            DBG(<< "texture alloc: " << (void*)unit->texture);
        }
        return unit->texture;
    } else {
        ERR(<< "use a invalid sdl_texture id");
        return nullptr;
    }
}

SDL_Texture *Texture_sdl_manager::set_texture(long long id, SDL_Renderer *prenderer, SDL_Surface *surface, SDL_BlendMode blend_mode) {
    if (texture_map.find(id) != texture_map.end()) {
        Texture_sdl_manager_unit *unit = &texture_map[id];
        if (unit->texture) {
            SDL_DestroyTexture(unit->texture);
            DBG(<< "texture destroy: " << (void*)unit->texture);
            WINDOW_MANAGER->update_texture_count(unit->prenderer, -1);
            unit->texture = nullptr;
            unit->valid = false;
        }
        unit->prenderer = prenderer;
        if (prenderer == nullptr) {
            unit->texture = nullptr;
            unit->valid = false;
            return nullptr;
        }
        unit->texture = SDL_CreateTextureFromSurface(prenderer, surface);
        if (!unit->texture) {
            ERR(<< "couldn't create the texture. SDL: " << SDL_GetError());
        } else {
            SDL_SetTextureBlendMode(unit->texture, blend_mode);
            unit->static_access = true;
            unit->valid = true;
            WINDOW_MANAGER->update_texture_count(unit->prenderer, 1);
            DBG(<< "texture alloc: " << (void*)unit->texture);
        }
        return unit->texture;
    } else {
        ERR(<< "use a invalid sdl_texture id");
        return nullptr;
    }
}

// SDL_Texture *Texture_sdl_manager::set_texture(long long id, SDL_Renderer *prenderer, SDL_Texture *pTexture) {
//     if (texture_map.find(id) != texture_map.end()) {
//         Texture_sdl_manager_unit *unit = &texture_map[id];
//         if (unit->texture) {
//             SDL_DestroyTexture(unit->texture);
//             DBG(<< "texture destroy: " << (void*)unit->texture);
//             unit->texture = nullptr;
//             unit->valid = false;
//         }
//         unit->prenderer = prenderer;
//         if (prenderer == nullptr) {
//             unit->texture = nullptr;
//             unit->valid = false;
//             return nullptr;
//         }
//         unit->texture = pTexture;
//         unit->valid = true;
//         return pTexture;
//     } else {
//         ERR(<< "use a invalid_sdl_texture_id");
//         return nullptr;
//     }
// }

bool Texture_sdl_manager::set_texture_blend_mode(long long id, SDL_BlendMode blend_mode) {
    if (texture_map.find(id) == texture_map.end()) {
        return false;
    }
    Texture_sdl_manager_unit *unit = &texture_map[id];
    if (unit->texture == nullptr) {
        return false;
    }
    SDL_SetTextureBlendMode(unit->texture, blend_mode);
    return true;
}

// bool Texture_sdl_manager::set_texture_update_func(long long id, std::function<SDL_Texture *()> func, bool auto_update_flag) {
//     if (texture_map.find(id) == texture_map.end()) {
//         return false;
//     }
//     Texture_sdl_manager_unit *unit = &texture_map[id];
//     unit->update_func = func;
//     unit->auto_update = auto_update_flag;
//     return true;
// }

// bool Texture_sdl_manager::set_auto_update(long long id, bool auto_update_flag) {
//     if (texture_map.find(id) == texture_map.end()) {
//         return false;
//     }
//     texture_map[id].auto_update = auto_update_flag;
//     return true;
// }

bool Texture_sdl_manager::is_valid(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        ERR(<< "invalid texture id");
        return false;
    }
    return texture_map[id].valid;
}

void Texture_sdl_manager::invalid_texture(SDL_Renderer *prenderer) {
    for (auto iter = texture_map.begin(); iter != texture_map.end(); ++iter) {
        if (iter->second.prenderer == prenderer && !iter->second.static_access) {
            iter->second.valid = false;
        }
    }
}

bool Texture_sdl_manager::destroy_texture(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        ERR(<< "invalid texture id");
        return false;
    }
    Texture_sdl_manager_unit *unit = &texture_map[id];
    SDL_DestroyTexture(unit->texture);
    unit->texture = nullptr;
    unit->valid = false; 
    WINDOW_MANAGER->update_texture_count(unit->prenderer, -1);
    return true;
}

void Texture_sdl_manager::invalid_texture(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        return;
    }
    texture_map[id].valid = false;
}
}