#ifdef __linux__

#include <alambre/system/linux/spi.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>

LinuxSpiBus::LinuxSpiBus(const char *dev_path) {
    // We have no way to signal failure here, so we'll just
    // prance on regardless.
    spi_fd = open(dev_path, O_RDWR);
    uint8_t bits_per_word = 8;
    ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
}

void LinuxSpiBus::shift_out(uint8_t value, BitOrder order) {
    // If we failed to open the fd in the constructor
    if (spi_fd < 0) return;

    spi_ioc_transfer spi;
    spi.rx_buf = NULL;
    spi.tx_buf = (unsigned long)&value;
    spi.len = 1;
    spi.delay_usecs = 0;
    spi.speed_hz = 0;
    spi.bits_per_word = 8;

    // FIXME: Check for errors and then signal them somehow?
    uint8_t lsb_first = (order == ISpiBus::MSB_FIRST ? 0 : 1);
    ioctl(spi_fd, SPI_IOC_WR_LSB_FIRST, &lsb_first);
    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
}

uint8_t LinuxSpiBus::shift_in(BitOrder order) {
    // If we failed to open the fd in the constructor
    if (spi_fd < 0) return 0;

    uint8_t value;

    spi_ioc_transfer spi;
    spi.rx_buf = (unsigned long)&value;
    spi.tx_buf = NULL;
    spi.len = 1;
    spi.delay_usecs = 0;
    spi.speed_hz = 0;
    spi.bits_per_word = 8;

    // FIXME: Check for errors and then signal them somehow?
    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

    return value;
}

void LinuxSpiBus::set_spi_mode(uint8_t mode) {
    // Map to the Kernel's values for these modes.
    switch (mode) {
        case 0: mode = SPI_MODE_0; break;
        case 1: mode = SPI_MODE_1; break;
        case 2: mode = SPI_MODE_2; break;
        case 3: mode = SPI_MODE_3; break;
    }

    // FIXME: Check for errors and signal them somehow?
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
}

void LinuxSpiBus::set_max_clock_freq_hz(uint32_t speed) {
    // FIXME: Check for errors and signal them somehow?
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

#endif

