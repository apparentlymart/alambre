#ifndef TENACIOUS_SYSTEM_AVR_GPIO
#define TENACIOUS_SYSTEM_AVR_GPIO

#ifndef __AVR_ARCH__
#error tenacious/system/avr/gpio.h may be used only in AVR builds.
#endif

#include <avr/io.h>
#include <tenacious/bus/gpio.h>

// We generate a separate class for each port using preprocessor macros
// because it's not clear to me yet how to (and whether it's possible to)
// pass the ports in as template arguments, since they don't have external
// linkage.
// This is really, really gross and I'd like to think there's a better
// way to do it but it's a start for now.

#define PINCLASS(PORTID) /** GPIO Implementation for pins on AVR port PORTID. This is an implementation detail of the AVR GPIO implementation. */ \
    template <char PIN> class AvrGpioPort##PORTID##Pin : public IGpioPin {\
      public:\
        inline void set(void) {\
            PORT##PORTID |= (1 << PIN);\
        }\
        inline void clear(void) {\
            PORT##PORTID &= ~(1 << PIN);\
        }\
        inline void write(const IGpioPin::PinValue val) {   \
            if (val == HIGH) {  \
                this->set();\
            }\
            else {\
                this->clear();\
            }\
        }\
    }

#ifdef PORTA
PINCLASS(A);
#endif
#ifdef PORTB
PINCLASS(B);
#endif
#ifdef PORTC
PINCLASS(C);
#endif
#ifdef PORTD
PINCLASS(D);
#endif
#ifdef PORTE
PINCLASS(E);
#endif
#ifdef PORTF
PINCLASS(F);
#endif
#ifdef PORTG
PINCLASS(G);
#endif
#ifdef PORTH
PINCLASS(H);
#endif
#ifdef PORTI
PINCLASS(I);
#endif
#ifdef PORTJ
PINCLASS(J);
#endif
#ifdef PORTK
PINCLASS(K);
#endif

#undef PINCLASS

#endif
