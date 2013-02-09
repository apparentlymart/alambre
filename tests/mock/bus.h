#ifndef ALAMBRE_MOCK_BUS
#define ALAMBRE_MOCK_BUS

#include <gmock/gmock.h>
#include <alambre/bus/gpio.h>
#include <alambre/bus/spi.h>

class MockGpioPin : public IGpioPin {

 public:
    MOCK_METHOD0(set, void());
    MOCK_METHOD0(clear, void());
    MOCK_METHOD1(write, void(IGpioPin::PinValue));
    MOCK_METHOD0(read, IGpioPin::PinValue());
    MOCK_METHOD1(set_direction, void(IGpioPin::PinDirection));
    MOCK_METHOD0(get_direction, IGpioPin::PinDirection());
};

class MockSpiBus : public ISpiBus {
 public:
    MOCK_METHOD2(shift_out, void(uint8_t value, ISpiBus::BitOrder order));
    MOCK_METHOD1(shift_in, uint8_t(ISpiBus::BitOrder order));
};

#endif
