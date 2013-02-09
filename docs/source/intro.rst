Introduction
============

Alambre is a C++ library for microcontrollers and other embedded systems.
Its main goal is providing a hardware abstraction allowing drivers to be
written for a variety of interesting devices and then used across all
supported platforms. For example, it should be possible to write a driver
for an Shift Register that speaks SPI and use it with a variety of different
SPI implementations without modification.

This is achieved by defining standard interfaces for standard buses and then
providing multiple compatible implementations of these interfaces. On top
of this we can also provide device-agnostic interfaces for functionality
provided by different devices so that the application developer can focus
on the unique functionality of their application rather than the
low-level details of the particular hardware they have.

All of this must of course be achieved without considerable code or memory
bloat so that it can be readily used in code running on microcontrollers.

Supported Platforms
-------------------

The primary target at this time is AVR microcontrollers. This is likely to
expand to other platforms once the AVR support is fleshed out a bit.
In principle it should be possible to target any platform supported by
GCC. Currently no effort is being made to support other C++ compilers,
since G++ provides some language extensions that are convenient for
the compile-time wiring techniques this library uses.

The non-hardware-specific parts of the library can also target a Linux
system to enable unit testing.

Compile-time Wiring with C++ Templates
--------------------------------------

The usual way to compose objects in C++ is to use run-time polymorphism via
virtual methods. This allows the precise type of an object to be determined
at runtime, which makes for a flexible program but has a penalty at runtime
and prevents certain compiler optimizations that could be performed if
method calls are bound at compile time.

Since embedded systems do not usually support runtime re-configuration of
wiring (you don't suddenly switch your LED from GPIO pin 1 to pin 2 while
the system is running), this library uses C++ Templates as a technique to
allow composition of objects at compile time. This allows more complete
program optimization, but does require some additional complexity in the
initialization code.

The general pattern is that any class that makes use of an interface must
be declared as a template that is parameterized by the concrete implementation
of that interface. For example, the interface ``IGpioPin`` describes the
generic functionality of a general-purpose IO pin, and this can be used
in the driver for a device that requires two GPIO pins:

.. code-block:: cpp

   template <class OUTPUT_PIN_TYPE, class CLOCK_PIN_TYPE>
   class SomeDevice {
     public:
       OUTPUT_PIN_TYPE *output_pin;
       CLOCK_PIN_TYPE *clock_pin;

       SomeDevice(OUTPUT_PIN_TYPE *output_pin, CLOCK_PIN_TYPE *clock_pin) {
           this->output_pin = output_pin;
           this->clock_pin = clock_pin;
           output_pin->set_direction(IGpioPin::OUTPUT);
           clock_pin->set_direction(IGpioPin::OUTPUT);
       }

       void do_something() {
           this->output_pin->set();
           this->clock_pin->set();
           this->clock_pin->clear();
           this->output_pin->clear();
           this->clock_pin->set();
           this->clock_pin->clear();
       }
   }

This can be "wired up" to a pair of GPIO pins on an AVR by passing the types
of the AVR pins as part of the declaration:

.. code-block:: cpp

   #include <alambre/system/avr.h>

   SomeDevice<typeof(*avr_system.B1), typeof(*avr_system.B2)>
       my_device(avr_system.B1, avr_system.B2);

   int main() {
       my_device.do_something();
   }

In most cases it's best to use the ``typeof`` operator to automatically
insert the correct types, since this allows the given variables to have
their types changed in future without requiring changes to every use
of those variables.

The extra type declarations in the declaration of ``my_device`` allow the
compiler to see that AVR GPIO pins are being used and it can then generate
the optimal code to access these pins: a single machine instruction for
each call to ``set`` or ``clear``:

.. code-block:: cpp-objdump

   00000000 <main>:
      0:    29 9a       sbi	0x05, 1	; 5
      2:    2a 9a       sbi	0x05, 2	; 5
      4:    2a 98       cbi	0x05, 2	; 5
      6:    29 98       cbi	0x05, 1	; 5
      8:    2a 9a       sbi	0x05, 2	; 5
      a:    2a 98       cbi	0x05, 2	; 5
      c:    80 e0       ldi	r24, 0x00	; 0
      e:    90 e0       ldi	r25, 0x00	; 0
     10:    08 95       ret

This template-based wiring technique does have a drawback to be aware of:
each unique set of types used in the template will cause a new copy of
the ``SomeDevice`` class to be generated, growing the size of the resulting
binary. In most cases this isn't a big deal since systems often have only
one instance of each kind of device. If this assumption isn't true for your
application you may wish to use more traditional virtual methods instead.

Library Architecture
--------------------

Since this library aims to create a sense of architecture in embedded systems,
it's worth spending a little time describing the intended architecture.

Embedded systems most often consist of a microcontroller and a set of other
components which are connected to the microcontroller via some kind of bus.

This leads to the following concerns that this library seeks to separate:

* The *system* is the device on which the program is running. It provides
  access to the capabilities of the device. For example, the *AVR system*
  provides convenient access to the GPIO pins, SPI bus and I2C bus of
  the AVR the code is running on.

* A *device* is some kind of equipment connected to the system. Examples of
  devices include shift registers, LCD screens, keypads, real-time clocks,
  and so forth.

* A *bus* is a mechanism by which devices connect to the system. Common
  examples of buses are plain old GPIO, SPI, I2C and UART. Some buses are
  built in to the system in use. Some buses are implemented in terms of
  other buses, such as a software ("bit-banging") implementation of SPI written
  in terms of a GPIO bus. Some buses are provided by external devices that
  are themselves connected via a bus, such as GPIO pins on an external
  shift register connected to the microcontroller with SPI.

* A *capability* is some kind of common purpose provided by many different
  devices. For example, a graphical LCD and an LED matrix both provide the
  capability to draw 2D graphics, and a GPS reciever and a real-time clock
  IC can both tell you the current time.

The most important set of abstractions in this library are those of the
standard buses, because these in turn allow a device driver to be implemented
such that it can run on lots of different systems without modification.
However, capabilities are also interesting to allow an application developer
to separate the concerns of application behavior (e.g. determining the
time and showing it on a screen) from the specifics of how that is achieved
in the underlying hardware (e.g. reading the time from a particular real time
clock IC and showing it on an array of four seven-segment LED
displays using a particular LED display driver IC) so that code can more
easily be re-used between different creations.

Asynchronous Event Handling
---------------------------

Embedded systems often make extensive use of interrupts for asynchronous event
notifications, but interrupt support is very inconsistent between systems and
so this library cannot easily hide system-specific details for event handling.

However, the most important thing is to separate the detection of the event
(which is often done by the hardware itself) from the action taken in response
to that event (which is often the responsibility of a specific device driver).
We achieve this through a design pattern rather than any specific library
feature.

This pattern is most easily explained via an example. Imagine a hypothetical
device driver that responds to changes of value a GPIO input pin. The GPIO pin
interface itself does not provide a mechanism to request notifications because
the details vary so much between systems, but the device driver itself can
provide a method that can be called to notify it of the state change, like
this:

.. code-block:: cpp

    template <class INPUT_PIN_TYPE>
    class SomeDevice {
     public:
       INPUT_PIN_TYPE *input_pin;

       SomeDevice(INPUT_PIN_TYPE *input_pin) {
           this->input_pin = input_pin;
           input_pin->set_direction(IGpioPin::INPUT);
       }

       inline void notify_input_pin_change() {
           IGpioPin::PinValue value = this->input_pin->read();
           // ... and then do something with the value
       }
    };

It is then the responsibility of the system-specific wiring code to connect
whatever event or interrupt signals the input pin change to the notify function.
For example, on an AVR:

.. code-block:: cpp

   #include <alambre/system/avr.h>

   SomeDevice<typeof(*avr_system.B1)>
       my_device(avr_system.B1);
   // Ideally the AVR system library would provide a less ugly way to wire this,
   // but this is just an example.
   ISR(INT0_vect) {
       my_device.notify_input_pin_change();
   }

The rules for this pattern are as follows:

* The documentation for a class that represents a device that emits an event
  must describe how to wire the event up to an event handler, and will ideally
  provide a convenient way to do so in one line of declaration code to avoid
  creating lots of noise in system-specific wiring code.

* A class that consumes an event must provide a method whose name begins with
  `notify_` and continues with a description of the intended event, as in
  the example above, and describe in the documentation of that method in
  what situations the method should be called.

* The notify method must be `void` and must take no parameters.

* The notify method should be declared as `inline` to increase the likelihood
  that the compiler will be able to embed its implementation directly into
  an interrupt service routine. This is particularly beneficial for AVR targets
  because avr-gcc can generate smaller code for an ISR that does not call any
  other functions.

* Likewise, the notify method should avoid calling any other functions where
  possible.

* The notify method should be as short as possible and should ideally mutate
  state only inside the instance the method is called on.

* Any variables that can be modified by the notify method must be declared
  `volatile` to let the compiler know they can be modified by asynchronous code,
  and care must be taken when mutating values that cannot be updated atomically.
