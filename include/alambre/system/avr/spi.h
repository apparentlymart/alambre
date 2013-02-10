#ifndef ALAMBRE_SYSTEM_AVR_SPI
#define ALAMBRE_SYSTEM_AVR_SPI

#ifndef __AVR_ARCH__
#error alambre/system/avr/spi.h may be used only in AVR builds.
#endif

#include <avr/io.h>

// Only compile this if we have SPSR defined, indicating that this
// system has a SPI Status Register and therefore supports SPI.
#ifdef SPSR

#include <alambre/bus/spi.h>

class AvrSpiBus : public ISpiBus {

  public:

    void shift_out(uint8_t value, BitOrder order) {

    }

    uint8_t shift_in(BitOrder order) {

    }

};

#endif
#endif