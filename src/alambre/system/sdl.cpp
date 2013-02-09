#ifdef SDL_DUMMIES

#include <alambre/system/sdl.h>

bool SdlSystem::initialized = false;
Uint32 SdlSystem::init_flags = 0;

void SdlSystem::initialize(Uint32 init_flags) {
    if (initialized) {
        SDL_Init(init_flags);
        SdlSystem::initialized = true;
        SdlSystem::init_flags = init_flags;
    }
    else {
        Uint32 new_init_flags = (~SdlSystem::init_flags) & init_flags;
        if (new_init_flags) {
            SDL_InitSubSystem(new_init_flags);
            SdlSystem::init_flags |= init_flags;
        }
    }
}

#endif
