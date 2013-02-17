#ifndef ALAMBRE_CAPABILITY_2DGRAPHICS
#define ALAMBRE_CAPABILITY_2DGRAPHICS

#include <stdint.h>

/**
   Interface for an immutable 2D bitmap.

   This interface is ideal for simple wrappers around static bitmaps in
   the program. For example, a bitmap stored as raw data in an AVR's program
   memory could be exposed via an implementation of this interface.
 */
template <class COORD_TYPE, class COLOR_TYPE>
class IBitmap2d {

  public:

    /**
     * The type used for coordinates in this bitmap.
     *
     * Since typedefs do not inherit, implementers of this interface must
     * redefine this.
     */
    typedef COORD_TYPE coord_type;

    /**
     * The type used for colors in this bitmap.
     *
     * Since typedefs do not inherit, implementers of this interface must
     * redefine this.
     */
    typedef COLOR_TYPE color_type;

    /**
     * Get the color of the pixel at the given coordinates.
     */
    color_type get_pixel(coord_type x, coord_type y);

    /**
     * Get the width of the bitmap in pixels.
     */
    coord_type get_width();

    /**
     * Get the height of the bitmap in pixels.
     */
    coord_type get_height();
};

/**
 * Interface for a mutable 2D bitmap.
 *
 * This is an extension of IBitmap2d that allows pixel colors to be changed.
 */
template <class COORD_TYPE, class COLOR_TYPE>
class IMutableBitmap2d : public IBitmap2d<COORD_TYPE, COLOR_TYPE> {

  public:

    typedef COORD_TYPE coord_type;
    typedef COLOR_TYPE color_type;

    /**
     * Set the color of the pixel at the given coordinates.
     */
    void set_pixel(coord_type x, coord_type y, color_type color);
};

/**
 * Simple, in-memory 2D bitmap.
 *
 * An implementation of IMutableBitmap2d that uses an in-RAM array of
 * COLOR_TYPE as data storage.
 */
template <class COORD_TYPE, class COLOR_TYPE, unsigned int WIDTH, unsigned int HEIGHT>
class Bitmap2d : public IMutableBitmap2d<COORD_TYPE, COLOR_TYPE> {

    COLOR_TYPE data[HEIGHT][WIDTH];

  public:

    typedef COORD_TYPE coord_type;
    typedef COLOR_TYPE color_type;

    inline void set_pixel(coord_type x, coord_type y, color_type color) {
        data[y][x] = color;
    }
    inline color_type get_pixel(coord_type x, coord_type y) {
        return data[y][x];
    }
    inline coord_type get_width() {
        return WIDTH;
    }
    inline coord_type get_height() {
        return HEIGHT;
    }
};

/**
 * Interface for an immutable 2D bitmap with one bit per pixel.
 *
 * This is an extension of IBitmap2d for bitmaps that use one bit per pixel,
 * with an additional method for accessing an entire byte of pixels at once.
 *
 * This interface assumes that the underlying bitmap is packed with horizontal
 * strips of eight pixels that fall directly on multiples of eight. It's not
 * appropriate for bit-per-pixel formats with different packing strategies.
 */
template <class COORD_TYPE>
class IBitPerPixelBitmap2d : public IBitmap2d<COORD_TYPE, bool> {

  public:

    typedef COORD_TYPE coord_type;
    typedef bool color_type;

    /**
     * Get a horizontal strip of eight pixels as a single bitmask.
     *
     * The strip starts at x * 8 pixels and proceeds right for eight pixels.
     *
     * For a display that has a compatible data packing strategy in its wire
     * protocol, reading and transmitting entire bytes will be more
     * efficient than reading individual pixels and re-packing them into
     * bytes for transmission.
     */
    uint8_t get_pixel_strip(coord_type xbyte, coord_type y);
};

/**
 * Interface for a mutable 2D bitmap with one bit per pixel.
 *
 * Extends IBitPerPixelBitmap2d with methods for writing entire bytes of
 * pixels at once.
 */
template <class COORD_TYPE>
class IMutableBitPerPixelBitmap2d : public IMutableBitmap2d<COORD_TYPE, bool>, public IBitPerPixelBitmap2d<COORD_TYPE> {
  public:

    typedef COORD_TYPE coord_type;
    typedef bool color_type;

    /**
     * Write a horizontal strip of eight pixels from a bitmask.
     */
    void write_pixel_strip(coord_type xbyte, coord_type y, uint8_t mask);
    /**
     * OR a horizontal strip of eight pixels from a bitmask.
     *
     * This is useful for drawing new shapes without disturbing the
     * surrounding pixels.
     */
    void or_pixel_strip(coord_type xbyte, coord_type y, uint8_t mask);
    /**
     * AND a horizontal strip of eight pixels from a bitmask.
     *
     * This is useful for drawing new shapes without disturbing the
     * surrounding pixels.
     */
    void and_pixel_strip(coord_type xbyte, coord_type y, uint8_t mask);
};

/**
 * In-memory 2D bitmap with a color palette.
 *
 * If the output device is true-color but there are 256 or fewer distinct
 * colors used for a particular application this class can be used as
 * an alternative to Bitmap2d to save RAM on the buffer at the expense
 * of an additional array read per pixel when updating the display.
 *
 * Use the COORD_TYPE and COLOR_TYPE of the target display when instantiating
 * the template. The pixel manipulation functions will actually use a uint8_t
 * index into the palette to represent color, but then the nested RenderBitmap
 * type (along with the instance at render_bitmap) can be passed to the
 * display update method to expose an immutable true-color bitmap for
 * rendering.
 */
template <class COORD_TYPE, class COLOR_TYPE, unsigned int WIDTH, unsigned int HEIGHT, uint8_t PALETTE_SIZE>
class PaletteBitmap2d : public Bitmap2d<COORD_TYPE, uint8_t, WIDTH, HEIGHT> {

    COLOR_TYPE *palette;

  public:

    typedef COORD_TYPE coord_type;
    typedef uint8_t color_type;

    /**
     * The type of colors stored in the palette.
     *
     * This is the color type of the render bitmap.
     */
    typedef COLOR_TYPE palette_entry_type;

    /**
     * Immutable true-color view on the bitmap.
     *
     * Provides an IBitmap2d-compatible interface to retrieve the true-color
     * image represented by the underlying image data and palette. This
     * is the appropriate type to use with the target display so that
     * the display will see the true colors rather than the palette indices.
     *
     * The palette lookups are done on the fly with each call to get_pixel.
     */
    class RenderBitmap : IBitmap2d<COORD_TYPE, COLOR_TYPE> {

        typedef PaletteBitmap2d<COORD_TYPE, COLOR_TYPE, WIDTH, HEIGHT, PALETTE_SIZE> real_bitmap_type;

        real_bitmap_type *real_bitmap;

        friend class PaletteBitmap2d<COORD_TYPE, COLOR_TYPE, WIDTH, HEIGHT, PALETTE_SIZE>;

      public:

        typedef COORD_TYPE coord_type;
        typedef COLOR_TYPE color_type;

        inline COLOR_TYPE get_pixel(coord_type x, coord_type y) {
            return real_bitmap->get_palette_value(real_bitmap->get_pixel(x, y));
        }

        inline coord_type get_width() {
            return WIDTH;
        }

        inline coord_type get_height() {
            return HEIGHT;
        }
    };

    /**
     * The render bitmap for this palette bitmap.
     *
     * This is just a view on the main bitmap with automatic palette lookup.
     * There is no additional pixel data stored in here.
     */
    RenderBitmap render_bitmap;

    /**
     * Create a new bitmap with the given palette.
     *
     * The pointer must be to an array with at least as many elements as
     * given in the PALETTE_SIZE template argument.
     */
    PaletteBitmap2d(COLOR_TYPE *palette) {
        this->palette = palette;
        this->render_bitmap.real_bitmap = this;
    }

    /**
     * Get the color in the given index of the palette array.
     */
    inline COLOR_TYPE get_palette_value(uint8_t index) {
        return palette[index];
    }

    /**
     * Set the color for the given index of the palette array.
     */
    inline void set_palette_value(uint8_t index, color_type color) {
        palette[index] = color;
    }

    /**
     * Determine the size of the palette, as given in the PALETTE_SIZE
     * template argument.
     */
    inline uint8_t get_palette_size() {
        // Currently we don't do anything with palette size except
        // expose it here for inspection, but this is intended to let
        // us make space-optimized implementations of this class in future,
        // like one that packs two pixels into each byte for when there
        // are 16 or fewer colors.
        return PALETTE_SIZE;
    }

};

/**
 * Capability interface for displays capable of showing 2D bitmaps.
 *
 * It is the caller's responsibility to ensure that the display has the
 * same pixel data format as the provided bitmap type.
 */
template <class BITMAP_TYPE>
class IBitmap2dDisplay {

  public:

    typedef typename BITMAP_TYPE::coord_type coord_type;
    typedef typename BITMAP_TYPE::color_type color_type;
    typedef BITMAP_TYPE bitmap_type;

    /**
     * Update the display to show the image stored in the given bitmap.
     */
    void update(BITMAP_TYPE *bitmap);

    /**
     * Update the display to show a region of the image stored in the given
     * bitmap.
     *
     * Some implementations may use the given update rectangle to optimize
     * the display update, but there is no guarantee that other parts of the
     * display will not also be updated, depending on the update procedure
     * for the device and the sophistication of the implementation.
     *
     * the x1 and y1 coordinates must *always* be less than or equal to the
     * corresponding x2 and y2; otherwise, the behavior is undefined.
     */
    void update(BITMAP_TYPE *bitmap, coord_type x1, coord_type y1, coord_type x2, coord_type y2);

};

#endif

