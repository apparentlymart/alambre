Linux
=====

Linux defines a convention for how a platform can provide user-space access to
GPIO pins, SPI buses and I2C buses. The Linux system provides bus
implementations that use these interfaces.

Not all platforms will necessarily implement these interfaces, but at least
the Linux driver for the BCM2708 chipset used in the Raspberry Pi implements
this.

The Linux interface is declared in the header ``alambre/system/linux.h``,
with the primary entry point being the ``linux_system`` variable described
below.

.. doxygenfile:: include/alambre/system/linux.h

GPIO
----

.. doxygenfile:: include/alambre/system/linux/gpio.h
