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

enum {ST_NONE=0, ST_CENTER=1, ST_UP, ST_DOWN, ST_LEFT, ST_RIGHT,
      ST_CLOSE, ST_OPEN, ST_BLINK};

enum {BLINK_BITMAP=0};
enum {CLOSE_SEQUENCE=0, OPEN_SEQUENCE, BLINK_SEQUENCE};

class Elwin
{
public:
    Elwin();
    ~Elwin();

    int init();
    int make();

    void make_eye_sequence(int eye_id, int bitmap_id, int sequence_id);
    void make_blink_bitmap_sequence(int eye_id, uint8_t* seq, size_t seq_size);

    void check_pupil_position();

    //int state;

#if defined(USE_SERVO)
    Servo servo[SERVO_COUNT];
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
