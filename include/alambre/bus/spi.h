#ifndef ALAMBRE_BUS_SPI
#define ALAMBRE_BUS_SPI

#include <stdint.h>
#include <alambre/bus/gpio.h>

/**
   Represents an SPI bus.

   The SPI *bus* represents the shared data and clock signals of the bus.
   A particular device on the bus also has a slave select signal, which
   is represented by the SpiChannel wrapper class.

   This is a generic interface that could be used for both master and slave
   SPI roles depending on the selected implementation.

   This interface does not provide a mechanism to control the data rate
   or specific pulsing behavior of the SPI bus; an implementation may provide
   a way to customize these details, or they may be fixed.
 */
class ISpiBus {
  public:

    enum BitOrder {
        LSB_FIRST = 1,
        MSB_FIRST = 2,
    };

    /**
       Transmit an 8-bit value on the SPI bus in the bit order given.
     */
    void shift_out(uint8_t value, BitOrder order);
    /**
       Recieve an 8-bit value on the SPI bus in the bit order given.

       In slave mode this will block until all eight bits have been recieved,
       with the rate of transmission under the control of the master.
     */
    uint8_t shift_in(BitOrder order);
};

/**
   A simple implementation of an SPI master in terms of a pair of GPIO pins.

   This implementation only supports transmission of data. The shift_in
   method is not implemented.

   This class builds an SPI bus in terms of the GPIO bus. Some systems may
   provide alternative implementations of SPI implemented within the
   microcontroller itself, but this implementation can in theory work
   anywhere two GPIO pins are available.

   This implementation is only able to operate in SPI master mode, and it
   will transmit as fast as the underlying system can execute the loop over
   the bits. On faster systems the transmission rate may be too fast for
   some devices.
 */
template <class OUTPUT_PIN_TYPE, class CLOCK_PIN_TYPE>
class SoftwareSpiMasterOutputBus : ISpiBus {

    OUTPUT_PIN_TYPE *output_pin;
    CLOCK_PIN_TYPE *clock_pin;

  public:

    SoftwareSpiMasterOutputBus(OUTPUT_PIN_TYPE *output_pin, CLOCK_PIN_TYPE *clock_pin) {
        this->output_pin = output_pin;
        this->clock_pin = clock_pin;
        output_pin->set_direction(IGpioPin::OUTPUT);
        clock_pin->set_direction(IGpioPin::OUTPUT);
        output_pin->clear();
        clock_pin->clear();
   }

    void shift_out(uint8_t value, BitOrder order) {
        for (uint8_t i = 0; i < 8; ++i) {
            if (order == LSB_FIRST) {
                this->output_pin->write(
                    !!(value & (1 << i)) ? IGpioPin::HIGH : IGpioPin::LOW
                );
            }
            else {
                this->output_pin->write(
                    !!(value & (1 << (7 - i))) ? IGpioPin::HIGH : IGpioPin::LOW
                );
            }
            // Pulse the clock pin.
            // FIXME: Might we need to introduce a delay in here for
            // the combination of fast microcontroller (really short pulse)
            // and slow device (needs a longer pulse)?
            this->clock_pin->set();
            this->clock_pin->clear();
        }
    }
};

/**
   Represents the combination of an SPI master bus and an SPI slave.

   Extends the ISpiBus interface with additional methods for selecting
   and deselecting the corresponding device. This can be used to encapsulate
   the entire configuration for communicating with a given device, leaving a
   device driver to simply send and recieve bytes.

   There are no checks to make sure that only one device on the bus is
   selected at a time. This is the caller's responsibility. It is good manners
   for a device driver using an SPI channel to only select its device
   for the duration of a specific function call, and deselect it before
   returning control to the caller.

   It doesn't make any sense to use this class with an SPI bus acting as
   a slave, since only the master role can select and deselect devices.
 */
template <class SPI_BUS_TYPE, class SELECT_PIN_TYPE>
class SpiChannel : ISpiBus {

    SPI_BUS_TYPE *spi_bus;
    SELECT_PIN_TYPE *select_pin;

  public:

    /**
       Create a new channel with the given bus and select pin.

       As a side-effect of the instantiation the pin will be configured as
       an output and set high to deselect the device.
     */
    SpiChannel(SPI_BUS_TYPE *spi_bus, SELECT_PIN_TYPE *select_pin) {
        this->spi_bus = spi_bus;
        this->select_pin = select_pin;

        // Select pin high means unselected
        select_pin->set_direction(IGpioPin::OUTPUT);
        select_pin->set();
    }

    inline void shift_out(uint8_t value, ISpiBus::BitOrder order) {
        this->spi_bus->shift_out(value, order);
    }

    inline uint8_t shift_in(ISpiBus::BitOrder order) {
        return this->spi_bus->shift_in(order);
    }

    /**
       Activate the remote slave in preparation for sending or recieving data.

       A device driver that calls this method as part of the implementation
       of one of its operations should always deselect the device before
       returning from that operation so the caller does not accidentally
       activate two devices on the bus simultaneously.
     */
    inline void select() {
        this->select_pin->clear();
    }

    /**
       Deactvate the remote device.

       For many devices this also serves to signal that a (possibly multi-byte)
       transmission is complete.
     */
    inline void deselect() {
        this->select_pin->set();
    }
};

#endif
