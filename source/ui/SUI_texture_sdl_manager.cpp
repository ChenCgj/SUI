#include "SDL_error.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SUI_in_texture_sdl_manager.h"
#include "SUI_in_debug.h"

namespace sui {

long long Texture_sdl_manager::curr_id = 1;

Texture_sdl_manager::Texture_sdl_manager() = default;

long long Texture_sdl_manager::alloc_texture_id() {
    Texture_sdl_manager_unit *unit = &texture_map[curr_id];
    unit->texture = nullptr;
    unit->prenderer = nullptr;
    unit->auto_update = false;
    unit->static_access = false;
    unit->update_func = nullptr;
    unit->id = curr_id;
    return curr_id++;
}

Texture_sdl_manager::~Texture_sdl_manager() {
    for (auto iter = texture_map.begin(); iter != texture_map.end(); ++iter) {
        if (iter->second.texture) {
            SDL_DestroyTexture(iter->second.texture);
            iter->second.texture = nullptr;
        }
    }
    DBG(<< "texture_sdl_manager delete ok");
}

Texture_sdl_manager *Texture_sdl_manager::instance() {
    static Texture_sdl_manager manager;
    return &manager;
}

bool Texture_sdl_manager::free_texture_id(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        return false;
    } else {
        Texture_sdl_manager_unit *unit = &texture_map[id];
        if (unit->texture) {
            SDL_DestroyTexture(unit->texture);
            unit->texture = nullptr;
        }
        texture_map.erase(id);
    }
    return true;
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
        }
        unit->prenderer = prenderer;
        if (prenderer == nullptr) {
            unit->texture = nullptr;
            return nullptr;
        }
        unit->texture = SDL_CreateTexture(prenderer, format, access, w, h);
        if (!unit->texture) {
            ERR(<< "couldn't create the texture. SDL: " << SDL_GetError());
        } else {
            unit->static_access = access == SDL_TEXTUREACCESS_STATIC;
            SDL_SetTextureBlendMode(unit->texture, blend_mode);
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
        }
        unit->prenderer = prenderer;
        if (prenderer == nullptr) {
            unit->texture = nullptr;
            return nullptr;
        }
        unit->texture = SDL_CreateTextureFromSurface(prenderer, surface);
        if (!unit->texture) {
            ERR(<< "couldn't create the texture. SDL: " << SDL_GetError());
        } else {
            SDL_SetTextureBlendMode(unit->texture, blend_mode);
            unit->static_access = true;
        }
        return unit->texture;
    } else {
        ERR(<< "use a invalid sdl_texture id");
        return nullptr;
    }
}

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

bool Texture_sdl_manager::set_texture_update_func(long long id, std::function<SDL_Texture *()> func, bool auto_update_flag) {
    if (texture_map.find(id) == texture_map.end()) {
        return false;
    }
    Texture_sdl_manager_unit *unit = &texture_map[id];
    unit->update_func = func;
    unit->auto_update = auto_update_flag;
    return true;
}

bool Texture_sdl_manager::set_auto_update(long long id, bool auto_update_flag) {
    if (texture_map.find(id) == texture_map.end()) {
        return false;
    }
    texture_map[id].auto_update = auto_update_flag;
    return true;
}

void Texture_sdl_manager::invalid_texture(SDL_Renderer *prenderer) {
    for (auto iter = texture_map.begin(); iter != texture_map.end(); ++iter) {
        if (iter->second.prenderer == prenderer && !iter->second.static_access) {
            if (iter->second.texture == nullptr) {
                continue;
            } else {
                SDL_DestroyTexture(iter->second.texture);
                if (iter->second.auto_update) {
                    iter->second.texture = iter->second.update_func();
                } else {
                    iter->second.texture = nullptr;
                }
            }
        }
    }
}

void Texture_sdl_manager::invalid_texture(long long id) {
    if (texture_map.find(id) == texture_map.end()) {
        return;
    }
    Texture_sdl_manager_unit *unit = &texture_map[id];
    if (unit->texture) {
        SDL_DestroyTexture(unit->texture);
        if (unit->auto_update) {
            unit->texture = unit->update_func();
        } else {
            unit->texture = nullptr;
        }
    }
}
}