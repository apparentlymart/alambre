#ifndef TENACIOUS_CAPABILITY_2DGRAPHICS
#define TENACIOUS_CAPABILITY_2DGRAPHICS

#include <stdint.h>

/**
   Interface for a 2D bitmap supporting pixel set operations and buffer flips.

   This is an interface intended to be implemented for a 2D display device
   that can then be used with the Graphics2d template class to perform
   higher-level graphics operations.

   The coordinate system for all implementations is two-dimensional with
   each dimension proceeding from zero at the top left. However actual hardware
   may be mounted at a different orientation than its driver expects, causing
   the coordinate system to appear rotated.

   The behavior when specifying pixels outside the bounds of the bitmap
   is undefined, but it should be avoided since it's likely to cause
   data corruption and possibly crashes on at least some implementations.
 */
template <typename COORD_TYPE, typename COLOR_TYPE>
class IBuffered2dGraphicsSurface {

 public:

    /**
       Typedef for the type used for coordinates.

       Exposed here for the benefit of wrapping classes, like Graphics.
    */
    typedef COORD_TYPE coord_type;

    /**
       Typedef for the type used for coordinates.

       Exposed here for the benefit of wrapping classes, like Graphics.
    */
    typedef COLOR_TYPE color_type;

    /**
       Set a particular pixel in the back-buffer to the given color.
    */
    void set_pixel(COORD_TYPE x, COORD_TYPE y, COLOR_TYPE color);


    /**
       Get the color of the given pixel in the back-buffer.
    */
    COLOR_TYPE get_pixel(COORD_TYPE x, COORD_TYPE y);

    /**
       Flip the back-buffer to the front-buffer for the given rectangular
       region.

       Implementations will update the given region as efficiently as possible.
       For example, an implementation for display hardware that only supports
       updating entire lines might ignore the x coordinates but update
       the range of rows given.
    */
    void flip(COORD_TYPE x1, COORD_TYPE x2, COORD_TYPE y1, COORD_TYPE y2);

    /**
       Get the closest color supported by this surface for the
       8-bit-per-channel RGB color given.

       This function only really makes sense for true color displays,
       and is likely to be unimplemented or implemented in a strange way
       for implementations that target low-color displays or displays whose
       colors are intrinsic rather than user-programmable.
     */
    COLOR_TYPE get_closest_color(unsigned char r, unsigned char g, unsigned char b);

    /**
       Get the width of the buffer in pixels.
     */
    COORD_TYPE get_width();

    /**
       Get the width of the buffer in pixels.
     */
    COORD_TYPE get_height();
};

/**
   Higher-level graphics utility class.

   Wraps a 2D graphics surface to provide higher-level drawing primitives
   like line segments and circles.
 */
template <typename SURFACE_TYPE>
class Graphics2d {

    SURFACE_TYPE *surface;

 public:

    /**
     * The coordinate type of the underlying graphics surface.
     */
    typedef typename SURFACE_TYPE::coord_type coord_type;
    /**
     * The color type of the underlying graphics surface.
     */
    typedef typename SURFACE_TYPE::color_type color_type;

 private:
    typedef typename SURFACE_TYPE::coord_type COORD_TYPE;
    typedef typename SURFACE_TYPE::color_type COLOR_TYPE;

    // Keeps track of the rectangular bounds in which we've
    // drawn since the last time we flipped so that we can
    // flip only the area we need to.
    COORD_TYPE inv_x1;
    COORD_TYPE inv_y1;
    COORD_TYPE inv_x2;
    COORD_TYPE inv_y2;

    void update_inv(COORD_TYPE x, COORD_TYPE y) {
        if (x < inv_x1) inv_x1 = x;
        if (y < inv_y1) inv_y1 = y;
        if (x > inv_x2) inv_x2 = x;
        if (y > inv_y2) inv_y2 = y;
    }

    void reset_inv() {
        inv_x1 = this->get_width() - 1;
        inv_y1 = this->get_height() - 1;
        inv_x2 = 0;
        inv_y2 = 0;
    }

 public:

    Graphics2d(SURFACE_TYPE *surface) {
        this->surface = surface;
        this->reset_inv();
    }

    /**
     * Set the given pixel in the back buffer to the given color.
     */
    inline void set_pixel(COORD_TYPE x, COORD_TYPE y, COLOR_TYPE color) {
        this->surface->set_pixel(x, y, color);
        this->update_inv(x, y);
    }

    /**
     * Get the color of the given pixel from the back buffer.
     */
    inline COLOR_TYPE get_pixel(COORD_TYPE x, COORD_TYPE y) {
        return this->surface->get_pixel(x, y);
    }

    /**
     * Set the width of the underlying surface.
     */
    inline COORD_TYPE get_width() {
        return this->surface->get_width();
    }

    /**
     * Set the height of the underlying surface.
     */
    inline COORD_TYPE get_height() {
        return this->surface->get_height();
    }

    /**
     * Copy to the front-buffer any changes made since the last call to flip.
     */
    void flip() {
        // If we've not actually drawn anything since last time
        // then our bounds will be "invalid", so do nothing.
        // (There is an edge case here where if either width or height
        // is 1 we'll always flip, but that's not worth worrying about.)
        if (this->inv_x2 <= this->inv_x1) {
            this->surface->flip(inv_x1, inv_y1, inv_x2, inv_y2);
            this->reset_inv();
        }
    }
};

/**
   Abstract class providing a common-case implementation of
   I2DBufferedGraphicsSurface with an in-memory backbuffer
   whose maximum size depends on the size of the int type
   on the target.

   The back-buffer is implemented as a two-dimentional array
   of the given COLOR_TYPE. This can be appropriate for RGB
   or other multi-color displays, but may not be best for
   displays with color formats that require less than one
   byte per pixel.

   Subclasses of this must implement the flip method
   taking an explicit rectangle.
 */
template <unsigned int WIDTH, unsigned int HEIGHT, typename COLOR_TYPE>
class AbstractBuffered2dGraphicsSurface : public IBuffered2dGraphicsSurface<unsigned int, COLOR_TYPE> {

  protected:
    /**
       The back-buffer used for intermediate pixel storage.

       Can be read directly by implementations of flip in subclasses.
     */
    COLOR_TYPE buf[HEIGHT][WIDTH];

  public:

    inline AbstractBuffered2dGraphicsSurface() {
        // Nothing to do
    }

    inline void set_pixel(unsigned int x, unsigned int y, COLOR_TYPE color) {
        this->buf[y][x] = color;
    }

    inline void get_pixel(unsigned int x, unsigned int y) {
        return this->buf[y][x];
    }

    inline unsigned int get_width() {
        return WIDTH;
    }

    inline unsigned int get_height() {
        return HEIGHT;
    }

    void flip(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
};

#endif

