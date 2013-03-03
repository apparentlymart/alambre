#ifdef __linux__

#include <alambre/system/linux/gpio.h>
#include <stdio.h>

void LinuxGpioPin::write(IGpioPin::PinValue value) {

    char path[256];
    sprintf(path, GPIO_VALUE_PATH_TEMPLATE, this->gpio_number);

    FILE * f = fopen(path, "w");
    if (f) {
        fputs((value == IGpioPin::LOW ? "0\n" : "1\n"), f);
        fclose(f);
    }
    else {
        // Not much we can do, sadly... this interface provides
        // no means for signalling an error.
        // FIXME: Maybe define a lightweight logging event and
        // have this call into it with an error message? That way
        // the app can log it to stderr if it wants, or just ignore
        // it if it doesn't care.
    }

}

void LinuxGpioPin::set_direction(IGpioPin::PinDirection direction) {

    char path[256];
    sprintf(path, GPIO_DIRECTION_PATH_TEMPLATE, this->gpio_number);

    FILE * f = fopen(path, "w");
    if (f) {
        fputs((direction == IGpioPin::OUTPUT ? "out\n" : "in\n"), f);
        fclose(f);
    }
    else {
        // As above, not much we can do here to signal an error.
    }

}

#endif
