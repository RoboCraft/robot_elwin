//
// definitions
//

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define SHOW_DEBUG_INFO  0

#define USE_SERVO       1
#define USE_LEDMATRIX   1

#if !defined(MOTORS_COUNT)
#define MOTORS_COUNT 2
#endif

#if !defined(SERVO_COUNT)
#define SERVO_COUNT 	3
#endif

// SPI SS pin
#define SPI_SS_PIN 53

#define HEAD_YAW_CENTER     90
#define HEAD_ROLL_CENTER    110
#define HEAD_PITCH_CENTER   90

struct Timings
{
    unsigned long blink_time;
    unsigned long loop_time;

    Timings() {
        blink_time = 0;
        loop_time = 0;
    }
};

// store motor pins
typedef struct MOTOR
{
  int in1;      // INPUT1
  int in2;      // INPUT2
  int enable; // ENABLE (pwm)
} MOTOR;

#define FORWARD HIGH
#define BACKWARD LOW

#define DEFAULT_SPEED 180

#endif //#ifndef _DEFINITIONS_H_
