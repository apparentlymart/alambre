AVR
===

The AVR series of microcontrollers by Atmel are supported using ``avr-g++``.
AVR-specific classes and functions are provided to expose the functionality
of the platform, including the GPIO ports, SPI interface, etc.

The AVR interface is declared in the header ``alambre/system/avr.h``,
with the primary entry point being the ``avr_system`` variable described
below.

.. doxygenfile:: include/alambre/system/avr.h
