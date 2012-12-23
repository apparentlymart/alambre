#ifndef TENACIOUS_DEVICE_MAX72XX
#define TENACIOUS_DEVICE_MAX72XX

#include <stdint.h>

/**
   Device driver for the MAX7219 and MAX7221 LED display driver ICs.

   These chips provide a convenient way to control up to 64 LEDs by
   multiplexing an 8x8 matrix. The chips can either produce digits on up to
   eight 7-segment numeric displays (decode mode) or directly drive an
   8x8 pixel matrix (no decode mode).
 */
template <class SPI_CHANNEL_TYPE>
class Max72xxDevice {
    SPI_CHANNEL_TYPE *spi_channel;

  public:
    Max72xxDevice(SPI_CHANNEL_TYPE *spi_channel) {
        this->spi_channel = spi_channel;
    }

    void set_register(uint8_t register_idx, uint8_t value) {
        this->spi_channel->select();
        this->spi_channel->shift_out(register_idx, ISpiBus::MSB_FIRST);
        this->spi_channel->shift_out(value, ISpiBus::MSB_FIRST);
        this->spi_channel->deselect();
    }

    inline void set_intensity(uint8_t intensity) {
        this->set_register(0x0A, intensity);
    }

    inline void set_scan_limit(uint8_t limit) {
        this->set_register(0x0B, limit);
    }

    inline void enable_display_test(void) {
        this->set_register(0x0F, 1);
    }

    inline void disable_display_test(void) {
        this->set_register(0x0F, 0);
    }

    inline void enable_display(void) {
        this->set_register(0x0C, 1);
    }

    inline void disable_display(void) {
        this->set_register(0x0C, 0);
    }

    inline void write_decode_mode(uint8_t mask) {
        this->set_register(0x09, mask);
    }

    inline void write_row(uint8_t row, uint8_t pattern) {
        // Since we don't do any checks on row < 8, this is almost
        // just an alias of set_register for now.
        this->set_register(row + 1, pattern);
    }

};

#endif

