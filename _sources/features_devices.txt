Devices
=======

A *device* is a piece of equipment that can be controlled from a
microcontroller, often via a *bus*. Classes in this category provide access
to the specific features of a given device, and may also provide
capability interface implementations for those devices that have a capability
that is shared with several devices.

MAX7219 and MAX7221 LED display driver ICs
------------------------------------------

These chips are manufactured by Maxim and  are both intended to drive up to
64 LEDs arranged in sets of eight. They can be used to control 8x8 pixel LED
matrices and also to control up to eight seven-segment LED displays.

The chips act as SPI slaves so the driver class requires an SPI master
implementation which it will use for signalling.

.. doxygenclass:: Max72xxDevice
   :members:


