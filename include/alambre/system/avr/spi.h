#ifndef ALAMBRE_SYSTEM_AVR_SPI
#define ALAMBRE_SYSTEM_AVR_SPI

#ifndef __AVR_ARCH__
#error alambre/system/avr/spi.h may be used only in AVR builds.
#endif

#include <avr/io.h>

// Only compile this if we have SPSR defined, indicating that this
// system has a SPI Status Register and therefore supports SPI.
#ifdef SPSR

#ifndef MOSI_DDR
#error avr-libc does not have MOSI_DDR defined for this target
#endif
#ifndef SCK_DDR
#error avr-libc does not have SCK_DDR defined for this target
#endif

#include <alambre/bus/spi.h>

class AvrSpiBus : public ISpiBus {

  public:

    AvrSpiBus() {
        MOSI_DDR |= 0x1 << MOSI_BIT;
        SCK_DDR |= 0x1 << SCK_BIT;
        SPCR |= (0x1 << SPE) | (0x1 << MSTR);
    }

    void shift_out(uint8_t value, BitOrder order) {
        if (ISpiBus::LSB_FIRST) {
            SPCR &= ~(0x1 << DORD);
        }
        else {
            SPCR |= (0x1 << DORD);
        }
        SPDR = value;
        while (!(SPSR & (1<<SPIF)));
    }

    uint8_t shift_in(BitOrder order) {

    }

};

#endif
#endif
