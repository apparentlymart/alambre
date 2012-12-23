Buses
=====

In libtenacious terminology, a *bus* is a mechanism for connecting devices
to the system. Interfaces are included for a few standard buses that are often
found in embedded systems. Device drivers will often use one or more bus
interfaces to interact with the corresponding device.

General Purpose IO (GPIO)
-------------------------

*GPIO* refers to the ability to turn on and off specific pins. Microcontrollers
often have GPIO *ports* that are collections of pins that are controlled by
the same byte (or word) in memory. At present there is no standard model for
GPIO ports, since these vary from system to system and are not often used
together as a unit, but individual pins are modelled by the
:cpp:class:`IGpioPin` interface which is often used by device drivers for
devices that require control of a single signal.

Specific implementations of GPIO pins are most often found via *system*
objects, which expose the built-in facilities of a given microcontroller or
embedded system.

.. doxygenclass:: IGpioPin
   :members:

Serial Peripheral Interface (SPI)
---------------------------------

The *Serial Peripheral Interface* is a protocol for synchronous data
communication between a master and one or more slaves. The master is the
device in control of the transmission rate and in control of which device
is selected to communicate on the bus at a given time.

Microcontrollers most often take the "master" role on the SPI bus, so most
SPI implementations implement only this part. One key example where this is
not true is when one microcontroller is communicating with another via SPI,
which of course means that one of them must at as the slave.

For the purposes of this library we separate the concept of the SPI *bus*,
the data and clock signals, from the concept of an SPI *channel*, with which
we model the master's interaction with a particular slave on the bus. When
the system is acting as a master and communicating with one or more slave
devices there will generally be one SPI *bus* and then one SPI *channel* for
each slave device on the bus.

The SPI bus is represented by the interface :cpp:class:`ISpiBus`. A channel
is represented by an instance of the wrapper class :cpp:class:`SpiChannel`,
which is parameterized by the SPI bus it belongs to and the GPIO pin that
will act as the *slave select* signal.

Finally, there is included a simple software implementation of SPI that
works in terms of GPIO pins and only supports output in the master role. This
is implemented in :cpp:class:`SoftwareSpiMasterOutputBus`. Hardware-specific
SPI implementations are made available via *system* objects.

.. doxygenclass:: ISpiBus
   :members:

.. doxygenclass:: SpiChannel
   :members:

.. doxygenclass:: SoftwareSpiMasterOutputBus
