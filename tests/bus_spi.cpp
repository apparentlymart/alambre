
#include "gtest/gtest.h"
#include "mock/bus.h"
#include "tenacious/bus/spi.h"

using ::testing::InSequence;

TEST(TestSoftwareSpi, Init) {
    // This is more of a compile-time test than a runtime test,
    // just exercising this wiring to make sure we can wire up
    // GPIOs to the SPI implementation.
    FakeGpioPin output_pin, clock_pin;
    SoftwareSpiMasterOutputBus<typeof(output_pin), typeof(clock_pin)>
        spi_bus(&output_pin, &clock_pin);

    // Do something to the object to make sure we don't optimize it away
    // completely and render this test a no-op.
    spi_bus.shift_out(0, ISpiBus::MSB_FIRST);
}

TEST(TestSoftwareSpi, ShiftOut) {
    InSequence dummy;
    MockGpioPin output_pin, clock_pin;

    // Constructor setup steps
    EXPECT_CALL(output_pin, set_direction(IGpioPin::OUTPUT));
    EXPECT_CALL(clock_pin, set_direction(IGpioPin::OUTPUT));
    EXPECT_CALL(output_pin, clear());
    EXPECT_CALL(clock_pin, clear());

    // ShiftOut MSB_FIRST Steps
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());

    // ShiftOut LSB_FIRST Steps
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::LOW));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());
    EXPECT_CALL(output_pin, write(IGpioPin::HIGH));
    EXPECT_CALL(clock_pin, set());
    EXPECT_CALL(clock_pin, clear());

    SoftwareSpiMasterOutputBus<typeof(output_pin), typeof(clock_pin)>
        spi_bus(&output_pin, &clock_pin);

    spi_bus.shift_out(0b10101010, ISpiBus::MSB_FIRST);
    spi_bus.shift_out(0b10101010, ISpiBus::LSB_FIRST);

}

TEST(TestSpiChannel, TestSelectAndShiftOut) {
    InSequence dummy;
    MockGpioPin select_pin;
    MockSpiBus spi_bus;

    // Constructor setup steps
    EXPECT_CALL(select_pin, set_direction(IGpioPin::OUTPUT));
    EXPECT_CALL(select_pin, set());

    // Select
    EXPECT_CALL(select_pin, clear());

    // Shift out
    EXPECT_CALL(spi_bus, shift_out(0b10101010, ISpiBus::MSB_FIRST));
    EXPECT_CALL(spi_bus, shift_out(0b01010101, ISpiBus::LSB_FIRST));

    // Deselect
    EXPECT_CALL(select_pin, set());

    SpiChannel<typeof(spi_bus), typeof(select_pin)>
        spi_channel(&spi_bus, &select_pin);

    spi_channel.select();
    spi_channel.shift_out(0b10101010, ISpiBus::MSB_FIRST);
    spi_channel.shift_out(0b01010101, ISpiBus::LSB_FIRST);
    spi_channel.deselect();

}
