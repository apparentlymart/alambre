#ifndef ALAMBRE_DEVICE_LPD8806
#define ALAMBRE_DEVICE_LPD8806

#include <stdint.h>
#include <alambre/bus/spi.h>
#include <alambre/capability/1dgraphics.h>

struct Lpd8806Color {

    uint8_t g;
    uint8_t r;
    uint8_t b;

  private:

    constexpr Lpd8806Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  public:
    constexpr Lpd8806Color() : r(0x80), g(0x80), b(0x80) {}

    constexpr static Lpd8806Color get_closest(uint8_t r, uint8_t g, uint8_t b) {
        return Lpd8806Color((r >> 1) | 0x80, (g >> 1) | 0x80, (b >> 1) | 0x80);
    }

    constexpr static Lpd8806Color get_raw(uint8_t r, uint8_t g, uint8_t b) {
        return Lpd8806Color(r, g, b);
    }
};

/**
   Device driver for LED strips based on the LPD8806 driver chip.

   This chip drives RGB LEDs at 7-bits-per-channel with automatic PWM.
   Many of these chips are often chained in a serial manner to create
   LED strips.

   Note that since this chip does not have a slave select pin this class
   is parameterized with an SPI \em bus implementation rather than an SPI
   \em channel.
 */
template <class SPI_BUS_TYPE>
class Lpd8806Device {
    SPI_BUS_TYPE *spi_bus;

  public:


    typedef Lpd8806Color raw_color;

    Lpd8806Device(SPI_BUS_TYPE *spi_bus) {
        this->spi_bus = spi_bus;
    }

    /**
       Converts an 8-bit-per-channel RGB color into a raw color value
       representing a 7-bit-per-channel RGB color.

       We half the color resolution in this process, with the "lost"
       colors being rounded down.
     */
    constexpr static raw_color get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        return raw_color::get_closest(r, g, b);
    }

    /**
       Transmit the color of a single pixel.

       The pixel colors for a strip are transmitted one pixel at a time
       with the pixel closest to the controller being transmitted first.
       A given pixel shows its new color only after the first byte of
       the next pixel is transmitted, or alternatively a reset sequence.
     */
    void transmit_color(raw_color color) {
        spi_bus->shift_out(color.g, ISpiBus::MSB_FIRST);
        spi_bus->shift_out(color.r, ISpiBus::MSB_FIRST);
        spi_bus->shift_out(color.b, ISpiBus::MSB_FIRST);
    }

    /**
       Transmit one or more reset sequences.

       A reset sequence is required in order to refresh an LED
       strip from the beginning again. One reset must be transmitted for
       each block of 32 LEDs in the strip.

       A reset can also be used to "latch" the last pixel in a chain; the
       pixels don't update to their new color until a subsequent byte is
       recieved, but that byte can be a reset if desired.
     */
    void transmit_reset(uint8_t times) {
        for (uint8_t i = 0; i < times; i++) {
            spi_bus->shift_out(0, ISpiBus::MSB_FIRST);
        }
    }

};

/**
   IBitmap1dDisplay capability implementation for LPD8006.

   Provides a straightforward way to buffer the color values in local
   memory and then refresh a whole strip in one go.

   Note that the LPD8006 updates each pixel as it is completed rather
   than waiting for the full set to update. Most of the time this isn't
   a problem because the strip can be updated faster than the human
   eye can see, but this might be problematic for very long strips.
 */
template <class BITMAP_TYPE, class LPD8806_TYPE, uint8_t NUM_RESETS=3>
class Lpd8806Bitmap1dDisplay : public IBitmap1dDisplay<BITMAP_TYPE> {

    LPD8806_TYPE *device;

  public:

    typedef typename BITMAP_TYPE::index_type index_type;
    typedef typename BITMAP_TYPE::color_type color_type;
    typedef BITMAP_TYPE bitmap_type;

    Lpd8806Bitmap1dDisplay(LPD8806_TYPE *device) {
        this->device = device;
        device->transmit_reset(NUM_RESETS);
    }

    void update(BITMAP_TYPE *bitmap) {
        index_type length = bitmap->get_length();
        for (index_type i = 0; i < length; i++) {
            device->transmit_color(bitmap->get_pixel(i));
        }
        device->transmit_reset(NUM_RESETS);
    }

    inline typename LPD8806_TYPE::raw_color get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        return this->device->get_closest_color(r, g, b);
    }

};

#endif
