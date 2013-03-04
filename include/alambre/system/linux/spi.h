#ifndef ALAMBRE_SYSTEM_LINUX_SPI
#define ALAMBRE_SYSTEM_LINUX_SPI

#ifndef __linux__
#error alambre/system/linux/spi.h may be used only in builds targeting Linux systems.
#endif

#include <stdint.h>
#include <alambre/bus/spi.h>

/**
 * ISpiBus implementation for access to the Linux devfs SPI interface.
 *
 * Although this is an implementation of ISpiBus, it's actually more like
 * an SPI channel in that it represents both a bus and a slave select pin.
 * However, the slave select pin is not independently controllable but rather
 * managed automatically as a side-effect of shifting a byte in or out.
 */
class LinuxSpiBus : public ISpiBus {

    int spi_fd;

  public:

    LinuxSpiBus(const char *dev_path);
    void shift_out(uint8_t value, BitOrder order);
    uint8_t shift_in(BitOrder order);

    /**
     * Set the SPI mode that will be used for transfers.
     *
     * Mode must be between 0 and 3, and follows the usual convention
     * for SPI modes.
     */
    void set_spi_mode(uint8_t mode);

    /**
     * Set the maximum clock frequency for transfers.
     *
     * The Linux kernel does not guarantee that transmissions will use exactly
     * the speed given.
     */
    void set_max_clock_freq_hz(uint32_t speed);

};

#endif
