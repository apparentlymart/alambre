
// This is just a placeholder to represent C++ sources while working on the
// Makefile.

#include <tenacious/bus/gpio.h>
#include <tenacious/system/avr.h>

int main() {
    avr_system.B5->set();
    avr_system.B7->clear();
    avr_system.C2->clear();
}
