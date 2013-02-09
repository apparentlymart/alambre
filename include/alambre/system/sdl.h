#ifdef SDL_DUMMIES
#ifndef ALAMBRE_SYSTEM_SDL
#define ALAMBRE_SYSTEM_SDL

#include <SDL.h>

/**
  A pseudo-system that is intended to provide the foundations of a dummy
  test environment running on a desktop system.

  The goal of this "system" is to enable development of dummy implementations
  of application capabilities that can run on a desktop system with SDL.
  For example, an application that displays graphics on an RGB LED matrix
  might have a dummy implementation that displays graphics on-screen on an
  SDL surface.
 */
class SdlSystem {

    static bool initialized;
    static Uint32 init_flags;

  public:

    /**
       Initialize SDL with the given flags.

       This is a thin wrapper around SDL_Init and SDL_InitSubSystem that
       keeps track of what has been initialized so far, thus allowing the
       other dummy implementations to just call in here unconditionally
       to make sure whatever they need is initialized, without having to
       coordinate with each other.
     */
    static void initialize(Uint32 init_flags);

};

#include <alambre/system/sdl/2dgraphics.h>

#endif
#endif // SDL_DUMMIES
