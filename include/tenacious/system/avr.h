#ifndef TENACIOUS_SYSTEM_AVR
#define TENACIOUS_SYSTEM_AVR

#ifndef __AVR_ARCH__
#error tenacious/system/avr.h may be used only in AVR builds.
#endif

#include <avr/io.h>
#include <tenacious/system/avr/gpio.h>
#include <tenacious/system/avr/spi.h>

#define TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(PORTID, PININDEX) AvrGpioPort##PORTID##Pin<PININDEX> * PORTID##PININDEX

/**
   Provides access to the built-in features of an AVR microcontroller.

   The members of this class will vary depending on which specific
   microcontroller unit is targeted by the current build.

   The GPIO pins are available as members with the names given in the
   unit's datasheet. The documentation for this class assumes a
   microcontroller with only GPIO port B available as an example; other
   ports will follow the same convention. Do not refer directly to the
   types of these members since it is considered an implementation
   detail; instead, use the typeof() operator.

 */
class AvrSystem {
 public:
    #ifdef PINA0
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 0);
    #endif
    #ifdef PINA1
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 1);
    #endif
    #ifdef PINA2
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 2);
    #endif
    #ifdef PINA3
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 3);
    #endif
    #ifdef PINA4
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 4);
    #endif
    #ifdef PINA5
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 5);
    #endif
    #ifdef PINA6
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 6);
    #endif
    #ifdef PINA7
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(A, 7);
    #endif

    #ifdef PINB0
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 0);
    #endif
    #ifdef PINB1
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 1);
    #endif
    #ifdef PINB2
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 2);
    #endif
    #ifdef PINB3
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 3);
    #endif
    #ifdef PINB4
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 4);
    #endif
    #ifdef PINB5
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 5);
    #endif
    #ifdef PINB6
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 6);
    #endif
    #ifdef PINB7
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(B, 7);
    #endif

    #ifdef PINC0
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 0);
    #endif
    #ifdef PINC1
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 1);
    #endif
    #ifdef PINC2
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 2);
    #endif
    #ifdef PINC3
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 3);
    #endif
    #ifdef PINC4
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 4);
    #endif
    #ifdef PINC5
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 5);
    #endif
    #ifdef PINC6
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 6);
    #endif
    #ifdef PINC7
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(C, 7);
    #endif

    #ifdef PIND0
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 0);
    #endif
    #ifdef PIND1
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 1);
    #endif
    #ifdef PIND2
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 2);
    #endif
    #ifdef PIND3
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 3);
    #endif
    #ifdef PIND4
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 4);
    #endif
    #ifdef PIND5
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 5);
    #endif
    #ifdef PIND6
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 6);
    #endif
    #ifdef PIND7
    TENACIOUS_SYSTEM_AVR_PIN_ACCESSOR(D, 7);
    #endif

    // TODO: More ports, so we have enough for the maximum number
    // of ports any AVR has.

    // Does this mcu support SPI?
    #ifdef SPSR
    AvrSpiBus *spi_bus;
    #endif

};

// Provide a global variable to access the AVR peripherals, but since
// everything we expose is inlined in practice this should be optimized
// away as unused in the final binary.
AvrSystem avr_system;

#endif
