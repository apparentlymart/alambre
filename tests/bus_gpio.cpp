
#include "gtest/gtest.h"
#include "mock/bus.h"
#include "alambre/bus/gpio.h"

using ::testing::InSequence;

TEST(TestGpio, FakeGpioPin) {

    FakeGpioPin pin;

    pin.set_direction(IGpioPin::OUTPUT);
    ASSERT_EQ(IGpioPin::OUTPUT, pin.get_direction());
    pin.set_direction(IGpioPin::INPUT);
    ASSERT_EQ(IGpioPin::INPUT, pin.get_direction());

    pin.set();
    ASSERT_EQ(IGpioPin::HIGH, pin.read());
    pin.clear();
    ASSERT_EQ(IGpioPin::LOW, pin.read());
    pin.write(IGpioPin::HIGH);
    ASSERT_EQ(IGpioPin::HIGH, pin.read());
    pin.write(IGpioPin::LOW);
    ASSERT_EQ(IGpioPin::LOW, pin.read());

}

TEST(TestGpio, NullGpioPin) {

    // The default null_gpio_pin instance is LOW and OUTPUT
    ASSERT_EQ(IGpioPin::LOW, null_gpio_pin.read());
    ASSERT_EQ(IGpioPin::OUTPUT, null_gpio_pin.get_direction());

    NullGpioPin<IGpioPin::HIGH> high_null_pin;
    ASSERT_EQ(IGpioPin::HIGH, high_null_pin.read());
    ASSERT_EQ(IGpioPin::OUTPUT, high_null_pin.get_direction());

    NullGpioPin<IGpioPin::HIGH, IGpioPin::INPUT> high_input_null_pin;
    ASSERT_EQ(IGpioPin::HIGH, high_input_null_pin.read());
    ASSERT_EQ(IGpioPin::INPUT, high_input_null_pin.get_direction());

}

