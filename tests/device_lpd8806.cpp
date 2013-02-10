
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
