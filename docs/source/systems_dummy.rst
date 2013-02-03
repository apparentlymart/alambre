Dummy Systems
=============

Some of the systems are provided not for production use but for use as dummies
during development on a desktop system. These systems provide implementations
of interfaces normally implemented on devices using facilities that are
commonly available on a desktop computer.

SDL Dummy System
----------------

The SDL dummy system uses the Simple DirectMedia Layer to provide certain
capability implementations.

The SDL system is not included in a build by default. To include it build
the library with the SDL_DUMMIES=1 parameter, and be sure to include libsdl
when linking.

2D Graphics
^^^^^^^^^^^

The SDL dummy system provides an implementation of the 2D graphics capability
in terms of SDL video. This can be used to try application code that is
written in terms of this capability.

.. doxygenclass:: WindowedSdl2dGraphicsSurface
   :members:

.. doxygenclass:: InMemorySdl2dGraphicsSurface
   :members:

