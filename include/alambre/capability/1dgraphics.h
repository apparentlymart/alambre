#ifndef ALAMBRE_CAPABILITY_1DGRAPHICS
#define ALAMBRE_CAPABILITY_1DGRAPHICS

#include <alambre/capability/2dgraphics.h>

/**
   Interface for a 1D bitmap supporting pixel set operations and buffer flips.

   This is an interface intended to be implemented for a 1-dimensional linear
   display device (e.g. an LED strip).

   The bitmap is addressed using indices from 0 to one less than the length.

   The behavior when specifying pixels outside the bounds of the bitmap
   is undefined, but it should be avoided since it's likely to cause
   data corruption and possibly crashes on at least some implementations.
 */
template <typename INDEX_TYPE, typename COLOR_TYPE>
class IBuffered1dGraphicsSurface {

 public:

    /**
       Typedef for the type used for indices.

       Exposed here for the benefit of wrapping classes.
    */
    typedef INDEX_TYPE index_type;

    /**
       Typedef for the type used for coordinates.

       Exposed here for the benefit of wrapping classes.
    */
    typedef COLOR_TYPE color_type;

    /**
       Set a particular pixel in the back-buffer to the given color.
    */
    void set_pixel(INDEX_TYPE index, COLOR_TYPE color);

    /**
       Get the color of the given pixel in the back-buffer.
    */
    COLOR_TYPE get_pixel(INDEX_TYPE index);

    /**
       Update the display to match what's in the back-buffer.
    */
    void flip();

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
       Get the length of the buffer.

       The largest index supported is (length - 1).
     */
    INDEX_TYPE get_length();

};

/**
   Abstract class providing a common-case implementation of
   I1dBufferedGraphicsSurface with an in-memory backbuffer
   whose maximum size depends on the size of the int type
   on the target.

   The back-buffer is implemented as an array of the given COLOR_TYPE.
   This can be appropriate for RGB or other multi-color displays, but
   may not be best for displays with color formats that require fewer than eight
   bits per pixel.

   Subclasses of this must implement the flip method.
 */
template <unsigned int LENGTH, typename COLOR_TYPE>
class AbstractBuffered1dGraphicsSurface : public IBuffered1dGraphicsSurface<unsigned int, COLOR_TYPE> {

  protected:
    /**
       The back-buffer used for intermediate pixel storage.

       Can be read directly by implementations of flip in subclasses.
     */
    COLOR_TYPE buf[LENGTH];

  public:

    inline AbstractBuffered1dGraphicsSurface() {
        // Nothing to do
    }

    inline void set_pixel(unsigned int index, COLOR_TYPE color) {
        this->buf[index] = color;
    }

    inline void get_pixel(unsigned int index) {
        return this->buf[index];
    }

    inline unsigned int get_length() {
        return LENGTH;
    }

    void flip();
};

/**
   Abstract class mapping the IBuffered2dGraphicsSurface capability to
   an implementation of IBuffered1dGraphicsSurface.

   This is a generic implementation with no specified mapping from 2D to
   1D coordinate spaces. Subclasses must implement map_coordinates to
   map 2D coordinates onto the 1D index space in some way.

   Subclasses must also implement get_width and get_height since only
   once the projection is defined can one define the rectangular space
   into which it projects.
 */
template <class BUFFERED_1D_SURFACE_TYPE, class ADAPTER_TYPE>
class AbstractBuffered2dTo1dGraphicsSurfaceAdapter : public IBuffered2dGraphicsSurface<typename BUFFERED_1D_SURFACE_TYPE::index_type, typename BUFFERED_1D_SURFACE_TYPE::color_type> {

  protected:
    BUFFERED_1D_SURFACE_TYPE *inner_surface;

  public:

    typedef BUFFERED_1D_SURFACE_TYPE inner_surface_type;
    typedef typename inner_surface_type::index_type index_type;
    typedef index_type coord_type;
    typedef typename inner_surface_type::color_type color_type;

    /**
       Take 2D coordinates and return the corresponding 1D index for those
       coordinates.

       This function provides the "projection" from the 2D coordinate system
       onto the underlying 1D coordinate system.
     */
    index_type map_coordinates(coord_type x, coord_type y);

    void set_pixel(coord_type x, coord_type y, color_type color) {
        index_type index = static_cast<ADAPTER_TYPE*>(this)->map_coordinates(x, y);
        //index_type index = this->ADAPTER_TYPE::map_coordinates(x, y);
        this->inner_surface->set_pixel(index, color);
    }

    color_type get_pixel(coord_type x, coord_type y) {
        index_type index = static_cast<ADAPTER_TYPE*>(this)->map_coordinates(x, y);
        return this->inner_surface->get_pixel(index);
    }

    inline color_type get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        return this->inner_surface->get_closest_color(r, g, b);
    }

    inline void flip(coord_type x1, coord_type y1, coord_type x2, coord_type y2) {
        this->inner_surface->flip();
    }

};

/**
 * Linear, left-to-right, top-to-bottom projection of 2D onto 1D.
 *
 * This implementation of AbstractBuffered2dTo1dGraphicsSurfaceAdapter
 * assumes that the 1D surface is sliced into rows that all go from
 * left to right, and then the rows themselves proceed from top to bottom.
 *
 * The length of the 1D surface should be a multiple of the given WIDTH.
 * If it isn't then the height will be rounded down to the nearest whole row.
 */
template <class BUFFERED_1D_SURFACE_TYPE, unsigned long WIDTH>
class RowSlicedBuffered2dTo1dGraphicsSurfaceAdapter : public AbstractBuffered2dTo1dGraphicsSurfaceAdapter<BUFFERED_1D_SURFACE_TYPE, RowSlicedBuffered2dTo1dGraphicsSurfaceAdapter<BUFFERED_1D_SURFACE_TYPE, WIDTH> > {

  private:
    typedef AbstractBuffered2dTo1dGraphicsSurfaceAdapter<BUFFERED_1D_SURFACE_TYPE, RowSlicedBuffered2dTo1dGraphicsSurfaceAdapter<BUFFERED_1D_SURFACE_TYPE, WIDTH> > base_type;

  public:

    // Forcefully "inherit" the typedefs from our parent, because
    // typedefs don't inherit automatically in C++.
    typedef typename base_type::inner_surface_type inner_surface_type;
    typedef typename base_type::index_type index_type;
    typedef typename base_type::coord_type coord_type;
    typedef typename base_type::color_type color_type;

    RowSlicedBuffered2dTo1dGraphicsSurfaceAdapter(BUFFERED_1D_SURFACE_TYPE *inner_surface) {
        this->inner_surface = inner_surface;
    }

    inline typename base_type::index_type get_width() {
        // Since WIDTH is a constant this conversion ought to happen
        // at compile time.
        return (typename base_type::index_type)(WIDTH);
    }

    inline typename base_type::index_type get_height() {
        // If the inner surface has a constant, inlined length then
        // this calculation ought to happen at compile time too.
        return this->inner_surface->get_length() / this->get_width();
    }

    inline typename base_type::index_type map_coordinates(coord_type x, coord_type y) {
        return (y * WIDTH) + x;
    }

};

#endif
