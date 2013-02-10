
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/bus.h"
#include "alambre/device/lpd8806.h"

using ::testing::InSequence;
using ::testing::Return;

TEST(TestDeviceLpd8806, Direct) {
    InSequence dummy;

    MockSpiBus spi_bus;
    Lpd8806Device<typeof(spi_bus)> strip(&spi_bus);

    // Test that color conversions work as expected.
    Lpd8806Device<typeof(spi_bus)>::raw_color test_color;
    test_color = strip.get_closest_color(255, 128, 2);
    ASSERT_EQ((255 >> 1) | 0x80, test_color.r);
    ASSERT_EQ((128 >> 1) | 0x80, test_color.g);
    ASSERT_EQ((2 >> 1) | 0x80, test_color.b);

    // Transmitting the color
    EXPECT_CALL(spi_bus, shift_out(test_color.g, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(test_color.r, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(test_color.b, ISpiBus::MSB_FIRST));

    // Transmitting the resets
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));

    strip.transmit_color(test_color);
    strip.transmit_reset(4);

}

TEST(TestDeviceLpd8806, Via1dGraphics) {

    InSequence dummy;

    MockSpiBus spi_bus;
    Lpd8806Device<typeof(spi_bus)> strip(&spi_bus);

    // Resets during construction (2 for long_surface, 1 for short_surface)
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));

    // Shifting out the four pixels of short_surface
    // (remember that the channels are transmitted in GRB order)
    // red
    EXPECT_CALL(spi_bus, shift_out(0x80, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0x80, ISpiBus::MSB_FIRST));
    // yellow
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0x80, ISpiBus::MSB_FIRST));
    // blue
    EXPECT_CALL(spi_bus, shift_out(0x80, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0x80, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    // white
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0xff, ISpiBus::MSB_FIRST));

    // and then one more reset on short_surface to latch the last pixel
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));

    Lpd8806Buffered1dGraphicsSurface<typeof(strip), 63> long_surface(&strip);
    Lpd8806Buffered1dGraphicsSurface<typeof(strip), 4> short_surface(&strip);

    auto red = short_surface.get_closest_color(255, 0, 0);
    auto yellow = short_surface.get_closest_color(255, 255, 0);
    auto blue = short_surface.get_closest_color(0, 0, 255);
    auto white = short_surface.get_closest_color(255, 255, 255);

    short_surface.set_pixel(0, red);
    short_surface.set_pixel(1, yellow);
    short_surface.set_pixel(2, blue);
    short_surface.set_pixel(3, white);

    short_surface.flip();

}
