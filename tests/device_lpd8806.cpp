
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/bus.h"
#include "alambre/capability/1dgraphics.h"
#include "alambre/device/lpd8806.h"
#include <stdint.h>

using ::testing::InSequence;
using ::testing::Return;

TEST(TestDeviceLpd8806, Direct) {
    InSequence dummy;

    MockSpiBus spi_bus;
    Lpd8806Device<typeof(spi_bus)> strip(&spi_bus);
    typedef typeof(strip) strip_type;

    // Test that color conversions work as expected.
    constexpr Lpd8806Device<typeof(spi_bus)>::raw_color test_color = strip_type::get_closest_color(255, 128, 2);
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
    typedef typeof(strip) strip_type;

    Bitmap1d<uint8_t, strip_type::raw_color, 4> bitmap;

    // Reset during construction (3 because we're using the default)
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));

    // Shifting out the four pixels of the bitmap
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

    // and then three more resets to latch the last pixel
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out('\0', ISpiBus::MSB_FIRST));

    Lpd8806Bitmap1dDisplay<typeof(bitmap), typeof(strip)> display(&strip);

    constexpr auto red = strip.get_closest_color(255, 0, 0);
    constexpr auto yellow = strip.get_closest_color(255, 255, 0);
    constexpr auto blue = strip.get_closest_color(0, 0, 255);
    constexpr auto white = strip.get_closest_color(255, 255, 255);

    bitmap.set_pixel(0, red);
    bitmap.set_pixel(1, yellow);
    bitmap.set_pixel(2, blue);
    bitmap.set_pixel(3, white);

    display.update(&bitmap);

}
