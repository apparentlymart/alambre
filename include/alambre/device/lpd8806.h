#ifndef ALAMBRE_DEVICE_LPD8806
#define ALAMBRE_DEVICE_LPD8806

#include <stdint.h>
#include <alambre/bus/spi.h>
#include <alambre/capability/1dgraphics.h>

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

    /**
       The type used for the raw representation of a color,
       storing the raw pixel data format of the device.

       Only the lower 7 bits of each are used for color data.
       The most significant bit must always be set for a valid color.
     */
    typedef struct {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    } raw_color;

    Lpd8806Device(SPI_BUS_TYPE *spi_bus) {
        this->spi_bus = spi_bus;
    }

    /**
       Converts an 8-bit-per-channel RGB color into a raw color value
       representing a 7-bit-per-channel RGB color.

       We half the color resolution in this process, with the "lost"
       colors being rounded down.
     */
    raw_color get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        raw_color ret;
        ret.g = (g >> 1) | 0x80;
        ret.r = (r >> 1) | 0x80;
        ret.b = (b >> 1) | 0x80;
        return ret;
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
   IBuffered1dGraphicsSurface capability implementation for LPD8006.

   Provides a straightforward way to buffer the color values in local
   memory and then refresh a whole strip in one go.

   Note that the LPD8006 updates each pixel as it is completed rather
   than waiting for the full set to update. Most of the time this isn't
   a problem because the strip can be updated faster than the human
   eye can see, but this might be problematic for very long strips.
 */
template <class LPD8806_TYPE, unsigned int LENGTH>
class Lpd8806Buffered1dGraphicsSurface : public AbstractBuffered1dGraphicsSurface<LENGTH, typename LPD8806_TYPE::raw_color> {

    LPD8806_TYPE *device;
    static const unsigned int NUM_RESETS = (LENGTH / 32) + 1;

  public:

    Lpd8806Buffered1dGraphicsSurface(LPD8806_TYPE *device) {
        this->device = device;
        device->transmit_reset(NUM_RESETS);
    }

    void flip() {
        for (unsigned int i = 0; i < LENGTH; i++) {
            device->transmit_color(this->buf[i]);
        }
        device->transmit_reset(NUM_RESETS);
    }

    inline typename LPD8806_TYPE::raw_color get_closest_color(unsigned char r, unsigned char g, unsigned char b) {
        return this->device->get_closest_color(r, g, b);
    }

};

#endif
