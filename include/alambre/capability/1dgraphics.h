#ifndef ALAMBRE_CAPABILITY_1DGRAPHICS
#define ALAMBRE_CAPABILITY_1DGRAPHICS

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

#endif
