Capabilities
============

A *capability* is an abstract feature or set of features that is implemented
in different ways by different hardware devices. Capabilities allow
application code to be cleanly separated from the specific hardware device
drivers to allow easier mocking, testing in a development environment, and
porting to new hardware.

2D Graphics
-----------

The *2D Graphics* capability represents 2D display devices of various kinds
that provide a rectangular matrix of pixels. The most abstract capability
interface can represent devices of all color types: single-color, bi-color,
RGB and anything in between.

This capability is split into two parts: the *2D graphics surface* interface,
:cpp:class:`IBuffered2dGraphicsSurface` provides a low-level double-buffered
device interface supporting only single-pixel operations, while the
graphics utility class, :cpp:class:`Graphics2d`, wraps the surface interface
to provide higher-level shape drawing such as line segments and circles.

.. doxygenclass:: IBuffered2dGraphicsSurface
   :members:

.. doxygenclass:: Graphics2d
   :members:

.. doxygenclass:: AbstractBuffered2dGraphicsSurface
   :members:
