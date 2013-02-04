#ifndef TENACIOUS_CAPABILITY_TIME
#define TENACIOUS_CAPABILITY_TIME

class IBcdRealTimeClock {

  public:

    class DateTime {
      public:
        char bcd_second;
        char bcd_minute;
        char bcd_hour;
        char day_of_week;
        char bcd_day_of_month;
        char bcd_month;
        char bcd_year;

        inline char get_unit_second() {
            return (this->bcd_second & 0b00001111);
        }
        inline char get_ten_second() {
            return (this->bcd_second & 0b01110000) >> 4;
        }
        inline char get_unit_minute() {
            return (this->bcd_minute & 0b00001111);
        }
        inline char get_ten_minute() {
            return (this->bcd_minute & 0b01110000) >> 4;
        }
        inline char get_unit_hour() {
            return (this->bcd_hour & 0b00001111);
        }
        inline char get_ten_hour() {
            char high_nybble = this->get_high_nybble_hour();

            // If we're in 12-hour mode then ignore the PM bit
            // because we have a separate function is_pm to read that.
            // This function just needs to return 0 or 1.
            if (high_nybble & 0b0100) high_nybble &= 0b1101;

            switch (high_nybble) {
                default: // default case should never happen
                case 0b100:  // 12-hour, 10 Hours clear
                case 0b000: return 0; // 24-hour, 20/10 Hours clear
                case 0b101:  // 12-hour, 10 hours set
                case 0b001: return 1; // 24-hour, 10 hours set
                case 0b010: return 2; // 24-hour, 20 hours set
            }
        }
        inline bool is_pm() {
            // true iff we're in 24-hour mode *and* the 20hour/PM bit is set
            return (this->bcd_hour & 0b0110000) == 0b0110000;
        }
        inline char get_high_nybble_hour() {
            return (this->bcd_hour & 0b01110000) >> 4;
        }
        inline char get_unit_day_of_month() {
            return (this->bcd_day_of_month & 0b00001111);
        }
        inline char get_ten_day_of_month() {
            return (this->bcd_day_of_month & 0b00110000) >> 4;
        }
        inline char get_unit_month() {
            return (this->bcd_month & 0b00001111);
        }
        inline char get_ten_month() {
            return (this->bcd_month & 0b00010000) >> 4;
        }
        inline char get_unit_year() {
            return (this->bcd_year & 0b00001111);
        }
        inline char get_ten_year() {
            return (this->bcd_year & 0b11110000) >> 4;
        }
        inline bool is_next_century() {
            return (this->bcd_month & 0b10000000);
        }
    };

    void read_all(DateTime *target);

    void read_hour_and_minute(DateTime *target);

    void read_time(DateTime *target);

    void read_date(DateTime *target);

    void write_all(DateTime *source);

    void write_time(DateTime *source);

    void write_hours_and_minutes(DateTime *source);

    void write_date(DateTime *source);

};

class IDecimalRealTimeClock {

    class DateTime {
      public:
        char second;
        char minute;
        char hour;
        char day_of_week;
        char day_of_month;
        char month;
        int year;
    };

    void read_all(DateTime *target);

    void read_hour_and_minute(DateTime *target);

    void read_time(DateTime *target);

    void read_date(DateTime *target);

    void write_all(DateTime *source);

    void write_time(DateTime *source);

    void write_hours_and_minutes(DateTime *source);

    void write_date(DateTime *source);
};

template <class DECIMAL_CLOCK_CLASS>
class DecimalToBcdRealTimeClockAdapter : IBcdRealTimeClock {
    // TODO: Implement this
};

template <class BCD_CLOCK_CLASS>
class BcdToDecimalRealTimeClockAdapter : IDecimalRealTimeClock {

    BCD_CLOCK_CLASS *bcd_clock;
    typename BCD_CLOCK_CLASS::DateTime source;
    int century;

    void convert_second(DateTime *target) {
        target->second = (
            this->source.get_ten_second() * 10
            + this->source.get_unit_second()
        );
    }

    void convert_minute(DateTime *target) {
        target->minute = (
            this->source.get_ten_minute() * 10
            + this->source.get_unit_minute()
        );
    }

    void convert_hour(DateTime *target) {
        target->hour = (
            (this->source.is_pm() ? 12 : 0)
            + this->source.get_ten_hour() * 10
            + this->source.get_unit_hour()
        );
    }

    inline void convert_day_of_week(DateTime *target) {
        target->day_of_week = this->source.day_of_week;
    }

    inline void convert_day_of_month(DateTime *target) {
        target->day_of_month = (
            this->source.get_ten_day_of_month() * 10
            + this->source.get_unit_day_of_month()
        );
    }

    inline void convert_month(DateTime *target) {
        target->month = (
            this->source.get_ten_month() * 10
            + this->source.get_unit_month()
        );
    }

    inline void convert_year(DateTime *target) {
        target->year = (
            this->century
            + (this->source.is_next_century() ? 100 : 0)
            + this->source.get_ten_year() * 10
            + this->source.get_unit_year()
        );
    }

    inline void convert_time(DateTime *target) {
        this->convert_second(target);
        this->convert_hour(target);
        this->convert_minute(target);
    }

    inline void convert_date(DateTime *target) {
        this->convert_day_of_week(target);
        this->convert_day_of_month(target);
        this->convert_month(target);
        this->convert_year(target);
    }

  public:

    BcdToDecimalRealTimeClockAdapter(BCD_CLOCK_CLASS *bcd_clock, int start_century) {
        this->bcd_clock = bcd_clock;
        this->century = start_century;
    }

    inline void read_all(DateTime *target) {
        this->bcd_clock->read_all(&(this->source));
        this->convert_time(target);
        this->convert_date(target);
    }

    inline void read_time(DateTime *target) {
        this->bcd_clock->read_time(&(this->source));
        this->convert_time(target);
    }

    inline void read_hour_and_minute(DateTime *target) {
        this->bcd_clock->read_hour_and_minute(&(this->source));
        this->convert_time(target);
    }

    inline void read_date(DateTime *target) {
        this->bcd_clock->read_date(&(this->source));
        this->convert_date(target);
    }

    // TODO: Implement write operations
};



#endif
