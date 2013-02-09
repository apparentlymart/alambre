Alambre is (or rather, is _intended to grow into_) a simple hardware
abstraction library for embedded applications development.

Introduction
============

This effort is a response to frustration with how library development is often
done in the AVR/Arduino/microcontroller community. Device driver libraries
are usually written to handle not only the specific functionality of the
device but also the details of the bus used to communicate with the device
and any variants possible in that communication.

For example, for almost every device that uses an SPI bus the driver library
includes code to choose (often at runtime!) between the AVR's hardware SPI
implementation and a bit-bang software implementation. This effort is
duplicated, the quality of the SPI implementation varies a lot, and the
resulting library ends up being useless if you want to run the same device
from an ARM system.

This library, therefore, is intended to grow into a simple set of hardware
abstractions that separate the following concerns:

* app-specific logic (provided by the app developer, not part of the library)
* interfaces modelling common behaviors independent of hardware (RGB 2D graphics, joysticks, etc)
* interfaces modelling the protocols for standard buses (SPI, I2C, UART, ...) and various plaform-specific implementations thereof.
* device-specific driver functionality

The primary motivation is allowing the device-specific driver functionality to
be reused across multiple platforms and benefit from a centrally-maintained
implementation of the bus protocols it needs. A secondary drive is allowing
application logic to be written in terms of device capabilities rather than
device details so that e.g. it's possible to provide mock implementations of
these for testing in a development environment away from the target device(s).

It is not a direct goal to allow an application written for one configuration
to be recompiled without source changes and immediately run on another
configuration, but the separation of concerns should allow an app developer
to support multiple targets in the same codebase with a moderate amount of
effort, and should certainly allow an application to be quickly ported from
one environment to another with only changes to the configuration and not
major changes to the main application logic.

General Design Principles
=========================

The following principles inform the design of this library,
in decreasing order of importance. Not all projects will agree with
this prioritization, but this is what this library targets.

Separation of concerns
----------------------

It's important to keep distinct the concerns of app-specific logic,
communication protocols, and device interactions.

For example, app knows it has a 2D RGB display of some size but doesn't care
how it's connected or what protocol is used to interact with it. The device
driver for a particular display knows that display uses SPI for communication
but doesn't care how the current platform implements SPI access.

Convenient Abstractions
-----------------------

This one's a tough one to get specific about since it's pretty subjective, but
one concrete point is that we prefer to use C++ features rather than C-style
object oriented programming, except for some specific techniques we avoid
because they either don't work or work poorly on microcontrollers.

Code Size and Memory Usage
--------------------------

Microcontrollers often have limited storage space for code, and even more
limited data RAM. Although compromises may occasionally be made to achieve
better separation of concerns, it's important not to build a library so big
that leaves no memory left for the application-specific logic.

Speed
-----

Some applcations are heavily speed-sensitive, such as those which bit-bang a
high-speed protocol. This library is not optimized for those cases, but should
not include code that is slower than necessary to achieve the above goals.
Smaller code size often encourages more speed anyway, but when the two are in
conflict code size will most often win the trade-off. Those who need raw speed
can of course go directly to the hardware features of their platform when
necessary, at the expense of separation of concerns.

Implementation Decisions
========================

The above design principles are a broad guideline of what you can expect to
find in this codebase. The following are some of the specific implementation
choices that fall out of these design principles.

Compile-time Configuration
--------------------------

In most embedded applications your AVR doesn't suddenly turn into a PIC at
runtime, or your button doesn't suddenly move from one GPIO pin to another.
Therefore we prefer to do configuration related to *wiring*, *device protocols*
and *device capabilities* at compile time.

We most often use C++ templates for user-configurable features, under the
assumption that most of the time you only have one kind of SPI bus in your
system and so it doesn't hurt that each different template instantiation
generates a new set of code because you wouldn't have more than one in the
first place.

In the uncommon cases where runtime reconfiguration is possible or when many
configurations of the same functionality are used at once, it should be
*possible* to provide your own implementation of the defined interfaces
supporting this flexibility while still benefiting from device driver code
written against the interfaces, but this won't be part of the core library.

Some things we can take care of automatically in the implementation by
watching for preprocessor macros defined by the compiler or system headers;
however, we will rarely encourage the *user* to define preprocessor macros
because their global scope encourages violation of separation of concerns.

Avoid Dynamic Memory Allocation
-------------------------------

We try as hard as possible to do all memory allocation either globally or on
the stack. Allocation on the heap is problematic for small microcontrollers
because their small data RAM is quickly exhausted and the small heap quickly
fragments with lots of allocation and deallocation.

If we must allocate memory dynamically, we do it as early as possible in the
program. For example, we could allocate memory inside the constructor of a
class if the class is expected to be instantiated on startup and never
destroyed. This memory will live at the start of the heap for the duration of
the program and therefore behave like global data in all the ways that matter.

No C++ features not available in AVR-GCC
----------------------------------------

AVR-GCC is probably the most important target of this library so we avoid the
features that are not available in this implementation:

* Don't use the C++ standard library, since it's not available in AVR-GCC.

* Don't use `new` and `delete` to allocate objects at runtime. (Along with these not being supported, it violates the principle of avoiding dynamic memory allocation)

* Don't use C++ exceptions. (compile with `-fno-exceptions` to guarantee this)

* Watch out that some of the C library header files are not C++ safe and need to be included inside an `extern "C"` block.


