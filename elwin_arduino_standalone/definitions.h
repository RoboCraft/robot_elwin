//
// definitions
//

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define SHOW_DEBUG_INFO  0

#define USE_SERVO       1
#define USE_LEDMATRIX   1

#if !defined(SERVO_COUNT)
#define SERVO_COUNT 	3
#endif

// SPI SS pin
#define SPI_SS_PIN 53

struct Timings
{
    unsigned long blink_time;
    unsigned long loop_time;

    Timings() {
        blink_time = 0;
        loop_time = 0;
    }
};

#endif //#ifndef _DEFINITIONS_H_
