#ifndef ALAMBRE_DEVICE_MAX72XX
#define ALAMBRE_DEVICE_MAX72XX

#include <stdint.h>
#include <type_traits>
#include <alambre/bus/spi.h>

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
    static_assert(
        std::is_base_of<ISpiBus, SPI_BUS_TYPE>::value,
        "SPI_BUS_TYPE must be a subclass of ISpiBus"
    );

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

#endif
