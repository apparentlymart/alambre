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

    /**
       Low-level operation to set a register value on the device.

       All other operations on this class are implemented in terms of
       this method. Consult the device datasheet for information on
       what registers are available.
     */
    void set_register(uint8_t register_idx, uint8_t value) {
        this->spi_channel->select();
        this->spi_channel->shift_out(register_idx, ISpiBus::MSB_FIRST);
        this->spi_channel->shift_out(value, ISpiBus::MSB_FIRST);
        this->spi_channel->deselect();
    }

    /**
       Set the intensity of the LEDs.

       Intensity is a value from 0 to 15 that controls the internal PWM
       of the chip, with higher values resulting in a brighter appearance.
     */
    inline void set_intensity(uint8_t intensity) {
        this->set_register(0x0A, intensity);
    }

    /**
       Set how many rows of eight LEDs will be scanned.

       The chips can be configured to scan from 1 to 8 rows of eight LEDs.
       When controlling 7-segment LED displays, this can be set to the
       number of digits.

       Since the chip illuminates one set of eight at a time in quick
       succession, reducing the scan limit can increase the brightness
       of the remaining digits, but be warned that this will increase
       the power dissipated by these digits. Consult the device datasheet
       for guidance.
     */
    inline void set_scan_limit(uint8_t limit) {
        this->set_register(0x0B, limit);
    }

    /**
       Enable the display test mode.

       In display test mode, all LEDs will be illumated regardless of what
       is set in the display data registers.
     */
    inline void enable_display_test(void) {
        this->set_register(0x0F, 1);
    }

    /**
       Disable the display test mode.

       This resumes normal operation after enabling test mode with
       enable_display_test.
     */
    inline void disable_display_test(void) {
        this->set_register(0x0F, 0);
    }

    /**
       Enable the display.

       By default the display is disabled to avoid the risk of garbled,
       random display during system boot. Call this method to make the
       display begin to show what's in the display registers.
     */
    inline void enable_display(void) {
        this->set_register(0x0C, 1);
    }

    /**
       Disable the display.

       This will turn off all LEDs in the display, but leave the display
       registers intact.
     */
    inline void disable_display(void) {
        this->set_register(0x0C, 0);
    }

    /**
       Configure the Decode Mode register.

       Decode Mode may be enabled for each row of eight LEDs individually
       and controls how the display driver interprets the content of each
       row's data register.

       When decode mode is enabled the register value will be interpreted as a
       number and rendered in a form that will cause the corresponding digit to
       be displayed on a 7-segment numeric display. When decode mode is
       disabled the register value is taken literally with each bit
       corresponding to one LED in the row.

       The parameter to this method is a bitmask where each bit corresponds
       to a row, and 1 enables decode mode while 0 disables it. To completely
       disable decode mode, pass 0x00.
     */
    inline void write_decode_mode(uint8_t mask) {
        this->set_register(0x09, mask);
    }

    /**
       Write a row to the display.

       How the provided pattern is interpreted depends on whether the
       row has *decode mode* enabled.

       Row is the row index to write, from 0 to 7. Be aware that there
       is no range checking on this parameter so values greater than 7
       will corrupt other display registers.
     */
    inline void write_row(uint8_t row, uint8_t pattern) {
        // Since we don't do any checks on row < 8, this is almost
        // just an alias of set_register for now.
        this->set_register(row + 1, pattern);
    }

};

#endif

