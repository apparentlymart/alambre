#ifndef ALAMBRE_SYSTEM_LINUX_GPIO
#define ALAMBRE_SYSTEM_LINUX_GPIO

#ifndef __linux__
#error alambre/system/linux/gpio.h may be used only in builds targeting Linux systems.
#endif

#include <alambre/bus/gpio.h>

/**
 * IGpioPin implementation for access to the Linux sysfs GPIO interface.
 */
class LinuxGpioPin : public IGpioPin {

    constexpr static const char * GPIO_VALUE_PATH_TEMPLATE = "/sys/class/gpio/gpio%i/value";
    constexpr static const char * GPIO_DIRECTION_PATH_TEMPLATE = "/sys/class/gpio/gpio%i/direction";

    int gpio_number;

  public:

    /**
     * Initialize this instance with the given GPIO number.
     *
     * The set of GPIO numbers is hardware-specific and so no special meaning
     * is assumed by this library. This GPIO pin must already have been
     * exported into sysfs either by a kernel driver or by some other
     * userspace process using /sys/class/gpio/export .
     */
    constexpr inline LinuxGpioPin(int gpio_number) : gpio_number(gpio_number) {}

    void write(IGpioPin::PinValue value);
    IGpioPin::PinValue read(void);

    inline void set(void) {
        this->write(IGpioPin::HIGH);
    }
    inline void clear(void) {
        this->write(IGpioPin::LOW);
    }

    void set_direction(PinDirection direction);
    PinDirection get_direction(void);

};

#endif
