#ifndef ALAMBRE_SYSTEM_AVR
#define ALAMBRE_SYSTEM_AVR

#ifndef __AVR_ARCH__
#error alambre/system/avr.h may be used only in AVR builds.
#endif

#include <avr/io.h>
#include <alambre/system/avr/gpio.h>
#include <alambre/system/avr/spi.h>

/**
   Macro for easily generating GPIO pin accessors.

   This is a private implementation detail.

   \private
 */
#define ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(PORTID, PININDEX) /** IGPIOPin implementation for pin PORTID##PININDEX */ AvrGpioPort##PORTID##Pin<PININDEX> * PORTID##PININDEX

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
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 0);
    #endif
    #ifdef PINA1
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 1);
    #endif
    #ifdef PINA2
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 2);
    #endif
    #ifdef PINA3
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 3);
    #endif
    #ifdef PINA4
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 4);
    #endif
    #ifdef PINA5
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 5);
    #endif
    #ifdef PINA6
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 6);
    #endif
    #ifdef PINA7
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(A, 7);
    #endif

    #ifdef PINB0
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 0);
    #endif
    #ifdef PINB1
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 1);
    #endif
    #ifdef PINB2
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 2);
    #endif
    #ifdef PINB3
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 3);
    #endif
    #ifdef PINB4
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 4);
    #endif
    #ifdef PINB5
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 5);
    #endif
    #ifdef PINB6
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 6);
    #endif
    #ifdef PINB7
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(B, 7);
    #endif

    #ifdef PINC0
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 0);
    #endif
    #ifdef PINC1
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 1);
    #endif
    #ifdef PINC2
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 2);
    #endif
    #ifdef PINC3
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 3);
    #endif
    #ifdef PINC4
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 4);
    #endif
    #ifdef PINC5
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 5);
    #endif
    #ifdef PINC6
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 6);
    #endif
    #ifdef PINC7
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(C, 7);
    #endif

    #ifdef PIND0
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 0);
    #endif
    #ifdef PIND1
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 1);
    #endif
    #ifdef PIND2
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 2);
    #endif
    #ifdef PIND3
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 3);
    #endif
    #ifdef PIND4
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 4);
    #endif
    #ifdef PIND5
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 5);
    #endif
    #ifdef PIND6
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 6);
    #endif
    #ifdef PIND7
    ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR(D, 7);
    #endif

    // TODO: More ports, so we have enough for the maximum number
    // of ports any AVR has.

    // Does this mcu support SPI?
    #ifdef SPSR
    /**
       ISpiBus implementation for the AVR's built-in SPI bus.

       This member is available only if the target AVR has an SPI bus.
     */
    AvrSpiBus *spi_bus;
    #endif

};

/**
   Global variable providing access to the features of the target AVR system.
 */
AvrSystem avr_system;

#undef ALAMBRE_SYSTEM_AVR_PIN_ACCESSOR

#endif
