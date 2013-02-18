#ifndef ALAMBRE_SYSTEM_SDL_2DGRAPHICS
#define ALAMBRE_SYSTEM_SDL_2DGRAPHICS

#include <SDL.h>
#include <alambre/capability/2dgraphics.h>
#include <alambre/system/sdl.h>

/**
   An abstract implementation of the IBuffered2dGraphicsSurface interface
   that renders pixels into an SDL surface.

   Subclasses must, in their constructors, assign to the "surface" data
   member an SDL surface that is WIDTH * PIXEL_SCALE pixels wide and
   HEIGHT * PIXEL_SCALE pixels tall.
 */
template <class BITMAP_TYPE, unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SCALE>
class AbstractSdlBitmap2dDisplay : public IBitmap2dDisplay<BITMAP_TYPE> {

  protected:

    SDL_Surface * surface;

  public:

    typedef typename BITMAP_TYPE::coord_type coord_type;
    typedef typename BITMAP_TYPE::color_type color_type;
    typedef BITMAP_TYPE bitmap_type;

    AbstractSdlBitmap2dDisplay() {
        SdlSystem::initialize(SDL_INIT_VIDEO);
    }

    virtual void update(BITMAP_TYPE *bitmap, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
        SDL_Rect rect;
        rect.w = PIXEL_SCALE;
        rect.h = PIXEL_SCALE;
        for (unsigned int y = y1; y <= y2; y++) {
            rect.y = y * PIXEL_SCALE;
            for (unsigned int x = x1; x <= x2; x++) {
                rect.x = x * PIXEL_SCALE;
                SDL_FillRect(
                    this->surface,
                    &rect,
                    bitmap->get_pixel(x, y)
                );
                SDL_Flip(this->surface);
            }
        }
    }

    virtual void update(BITMAP_TYPE *bitmap) {
        return this->update(bitmap, 0, 0, WIDTH - 1, HEIGHT - 1);
    }

    inline Uint32 get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        return SDL_MapRGB(this->surface->format, r, g, b);
    }

  protected:

    virtual SDL_Surface * create_surface() = 0;

};

/**
   An implementation of the IBuffered2dGraphicsSurface interface that renders
   pixels to an off-screen SDL surface.

   Since this class calls SDL_CreateRGBSurface as part of its constructor
   it's important to be sure to have already called SDL_SetVideoMode before
   instantiating this.
*/
template <class BITMAP_TYPE, unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SCALE>
class InMemorySdlBitmap2dDisplay : public AbstractSdlBitmap2dDisplay<BITMAP_TYPE, WIDTH, HEIGHT, PIXEL_SCALE> {

  public:

    InMemorySdlBitmap2dDisplay() {
        this->surface = this->create_surface();
    }
    ~InMemorySdlBitmap2dDisplay() {
        SDL_FreeSurface(this->surface);
    }

  protected:

    SDL_Surface * create_surface() {
        return SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            WIDTH * PIXEL_SCALE,
            HEIGHT * PIXEL_SCALE,
            32,
            0xff000000,
            0x00ff0000,
            0x0000ff00,
            0
        );
    }

};

/**
   An implementation of the IBuffered2dGraphicsSurface interface that renders
   pixels to an SDL window.

   The SDL window will be created as part of the constructor but it's the
   caller's responsibility to run an SDL event loop to keep the window
   on-screen and updated.
*/
template <class BITMAP_TYPE, unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SCALE=1>
class WindowedSdlBitmap2dDisplay : public AbstractSdlBitmap2dDisplay<BITMAP_TYPE, WIDTH, HEIGHT, PIXEL_SCALE> {

  public:

    WindowedSdlBitmap2dDisplay() {
        this->surface = this->create_surface();
    }

  protected:
    SDL_Surface * create_surface() {
        return SDL_SetVideoMode(
            WIDTH * PIXEL_SCALE,
            HEIGHT * PIXEL_SCALE,
            32,
            SDL_SWSURFACE
        );
    }

};

#endif
