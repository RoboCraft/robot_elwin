//
// robot Elwin
//
// http://robocraft.ru
//

#ifndef ELWIN_ARDUINO_H
#define ELWIN_ARDUINO_H

#include "definitions.h"
#include "spi_led_matrix.h"
#if defined(USE_SERVO)
#include <Servo.h>
#endif

// actions states:
enum {ACT_SLEEP=0, ACT_LOOKING, ACT_SQUINT, ACT_MODEST};

// eyes states:
enum {ST_NONE=0, ST_CENTER=1, ST_UP, ST_DOWN, ST_LEFT, ST_RIGHT,
      ST_CLOSE, ST_OPEN, ST_BLINK, ST_SQUINT};

enum {BLINK_BITMAP=0};
enum {CLOSE_SEQUENCE=0, OPEN_SEQUENCE, BLINK_SEQUENCE};

class Elwin
{
public:
    Elwin();
    ~Elwin();

    int init();
    int make();
    int make_action();

    void make_eye_sequence(int eye_id, int bitmap_id, int sequence_id);
    void make_blink_bitmap_sequence(int eye_id, uint8_t* seq, size_t seq_size);

    void check_pupil_position();

    //int state;

    int action_state;
    int act_counter;
    int blink_countdown; // countdown to next blink
    int pupil_countdown;
    int newX; int newY;

#if defined(USE_SERVO)
    Servo servo[SERVO_COUNT];

    int head_yaw;
    int head_roll;
    int head_pitch;

    int new_yaw;
    int new_roll;
    int new_pitch;

    int head_countdown;
    int head_action_countdown;
#endif

    SPI_LedMatrix eyes;

    int eyes_state;
    int counter;
    int idx;
    int eyeX;
    int eyeY;
    int eye_id;

private:

};

#endif // #ifndef ELWIN_ARDUINO_H
