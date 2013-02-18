#ifndef ALAMBRE_CAPABILITY_1DGRAPHICS
#define ALAMBRE_CAPABILITY_1DGRAPHICS

#include <alambre/capability/2dgraphics.h>

template <class INDEX_TYPE, class COLOR_TYPE>
class IBitmap1d {

  public:

    /**
     * The type used for indices in this bitmap.
     *
     * Since typedefs do not inherit, implementers of this interface must
     * redefine this.
     */
    typedef INDEX_TYPE index_type;

    /**
     * The type used for colors in this bitmap.
     *
     * Since typedefs do not inherit, implementers of this interface must
     * redefine this.
     */
    typedef COLOR_TYPE color_type;

    /**
     * Get the length of the bitmap in pixels.
     */
    color_type get_pixel(index_type index);

    /**
     * Get the length of the bitmap in pixels.
     */
    index_type get_length();

};

template <class INDEX_TYPE, class COLOR_TYPE>
class IMutableBitmap1d : public IBitmap1d<INDEX_TYPE, COLOR_TYPE> {

  public:

    typedef INDEX_TYPE index_type;
    typedef COLOR_TYPE color_type;

    void set_pixel(index_type index, color_type color);

};


template <class INDEX_TYPE, class COLOR_TYPE, unsigned int LENGTH>
class Bitmap1d : public IMutableBitmap1d<INDEX_TYPE, COLOR_TYPE> {

    COLOR_TYPE data[LENGTH];

  public:

    typedef INDEX_TYPE index_type;
    typedef COLOR_TYPE color_type;

    inline void set_pixel(index_type index, color_type color) {
        data[index] = color;
    }
    inline color_type get_pixel(index_type index) {
        return data[index];
    }
    inline index_type get_length() {
        return LENGTH;
    }

};

template <class INDEX_TYPE, class COLOR_TYPE, unsigned int LENGTH, uint8_t PALETTE_SIZE>
class PaletteBitmap1d : public Bitmap1d<INDEX_TYPE, uint8_t, LENGTH> {

    COLOR_TYPE *palette;

  public:

    typedef INDEX_TYPE index_type;
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
    class RenderBitmap : IBitmap1d<INDEX_TYPE, COLOR_TYPE> {

        typedef PaletteBitmap1d<INDEX_TYPE, COLOR_TYPE, LENGTH, PALETTE_SIZE> real_bitmap_type;

        real_bitmap_type *real_bitmap;

        friend class PaletteBitmap1d<INDEX_TYPE, COLOR_TYPE, LENGTH, PALETTE_SIZE>;

      public:

        typedef INDEX_TYPE index_type;
        typedef COLOR_TYPE color_type;

        inline color_type get_pixel(index_type index) {
            return real_bitmap->get_palette_value(real_bitmap->get_pixel(index));
        }

        inline index_type get_length() {
            return LENGTH;
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
    PaletteBitmap1d(COLOR_TYPE *palette) {
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

template <class BITMAP1D_TYPE, class ADAPTER_TYPE>
class AbstractMutableBitmap1dAsBitmap2dAdapter : public IMutableBitmap2d<typename BITMAP1D_TYPE::index_type, typename BITMAP1D_TYPE::color_type> {

  protected:
    BITMAP1D_TYPE *bitmap1d;

  public:

    typedef BITMAP1D_TYPE bitmap1d_type;
    typedef typename BITMAP1D_TYPE::index_type coord_type;
    typedef typename BITMAP1D_TYPE::index_type index_type;
    typedef typename BITMAP1D_TYPE::color_type color_type;

    index_type map_coordinates(coord_type x, coord_type y);

    inline color_type get_pixel(coord_type x, coord_type y) {
        index_type index = static_cast<ADAPTER_TYPE*>(this)->map_coordinates(x, y);
        return this->bitmap1d->get_pixel(index);
    }

    inline color_type set_pixel(coord_type x, coord_type y, color_type color) {
        index_type index = static_cast<ADAPTER_TYPE*>(this)->map_coordinates(x, y);
        this->bitmap1d->set_pixel(index, color);
    }

};


template <class BITMAP1D_TYPE, unsigned int WIDTH>
class RowSlicedMutableBitmap1dAsBitmap2dAdapter : public AbstractMutableBitmap1dAsBitmap2dAdapter<BITMAP1D_TYPE, RowSlicedMutableBitmap1dAsBitmap2dAdapter<BITMAP1D_TYPE, WIDTH> > {

  public:

    typedef BITMAP1D_TYPE bitmap1d_type;
    typedef typename BITMAP1D_TYPE::index_type coord_type;
    typedef typename BITMAP1D_TYPE::index_type index_type;
    typedef typename BITMAP1D_TYPE::color_type color_type;

    RowSlicedMutableBitmap1dAsBitmap2dAdapter(bitmap1d_type *bitmap1d) {
        this->bitmap1d = bitmap1d;
    }

    index_type map_coordinates(coord_type x, coord_type y) {
        return (y * WIDTH) + x;
    }

    inline coord_type get_width() {
        return (coord_type)WIDTH;
    }

    inline coord_type get_height() {
        return this->bitmap1d->get_length() / this->get_width();
    }

};


/**
 * Capability interface for displays capable of showing 1D bitmaps.
 *
 * It is the caller's responsibility to ensure that the display has the
 * same pixel data format as the provided bitmap type.
 */
template <class BITMAP_TYPE>
class IBitmap1dDisplay {

  public:

    typedef typename BITMAP_TYPE::index_type index_type;
    typedef typename BITMAP_TYPE::color_type color_type;
    typedef BITMAP_TYPE bitmap_type;

    /**
     * Update the display to show the image stored in the given bitmap.
     */
    void update(BITMAP_TYPE *bitmap);

};

#endif

