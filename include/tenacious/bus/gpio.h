
/**
   Represents a particular GPIO pin.

   On many systems a GPIO pin is part of a GPIO port that consists of 8, 16 or
   32 pins. This interface represents only one such pin, and is useful for
   configuring device driver classes that make use of GPIOs.
 */
class IGpioPin {
 public:

    /**
       Possible values of a pin.
     */
    enum PinValue {
        LOW = 0,
        HIGH = 1,
    };
    /**
       Possible directions of a pin.
     */
    enum PinDirection {
        INPUT = 0,
        OUTPUT = 1,
    };

    /**
      Set the pin's value to HIGH.

      This is only meaningful for pins configured for the OUTPUT direction
      and may have other hardware-dependent consequences on INPUT pins.
     */
    void set(void);
    /**
      Set the pin's value to LOW.

      This is only meaningful for pins configured for the OUTPUT direction
      and may have other hardware-dependent consequences on INPUT pins.
     */
    void clear(void);
    /**
       Write the given value to the pin.

      This is only meaningful for pins configured for the OUTPUT direction
      and may have other hardware-dependent consequences on INPUT pins.
     */
    void write(PinValue value);
    /**
       Determine the current value of the pin.

       For some hardware this may work only for pins configured for the
       INPUT direction.
     */
    PinValue read(void);

    /**
       Configure the direction of the pin.

       Not all pins are direction-configurable. An implementation of this
       method will not be provided on such hardware.
     */
    void set_direction(PinDirection direction);
    /**
       Determine the current direction of the pin.

       On hardware with configurable pin direction this will return the
       configured pin direction. On systems where pin direction is forced,
       this will return the forced direction.
     */
    PinDirection get_direction(void);

    // TODO: Interrupts?
};

