#ifndef ALAMBRE_SYSTEM_LINUX
#define ALAMBRE_SYSTEM_LINUX

#ifndef __linux__
#error alambre/system/linux.h may be used only in builds targeting Linux systems.
#endif

#include <alambre/system/linux/gpio.h>

/**
 * System targeting the Linux user-space GPIO, SPI and I2C interfaces.
 *
 * This provides a set of generic implementations that should work on any
 * Linux system with standard GPIO, SPI and I2C driver interfaces. Devices are
 * controlled via special files in sysfs and devfs.
 *
 * Some platforms may have more specialized alternative interfaces available
 * that could yield better performance, such as direct access to memory-mapped
 * I/O registers. This generic system does not use these.
 */
class LinuxSystem {

  public:
    constexpr inline LinuxGpioPin get_gpio_pin(int gpio_number) {
        return LinuxGpioPin(gpio_number);
    }

};

constexpr LinuxSystem linux_system;

#endif

