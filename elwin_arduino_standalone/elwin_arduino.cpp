//
// robot Elwin
//
// http://robocraft.ru
//

#include "elwin_arduino.h"

//
// https://learn.adafruit.com/animating-multiple-led-backpacks
//
// https://github.com/adafruit/Adafruit-LED-Backpack-Library/blob/master/examples/roboface/roboface.pde
//
static const uint8_t PROGMEM // Bitmaps are stored in program memory
blinkImg[][8] = { // Eye animation frames
                  { B00111100,         // Fully open eye
                    B01111110,
                    B11111111,
                    B11111111,
                    B11111111,
                    B11111111,
                    B01111110,
                    B00111100 },
                  { B00000000,
                    B01111110,
                    B11111111,
                    B11111111,
                    B11111111,
                    B11111111,
                    B01111110,
                    B00111100 },
                  { B00000000,
                    B00000000,
                    B00111100,
                    B11111111,
                    B11111111,
                    B11111111,
                    B00111100,
                    B00000000 },
                  { B00000000,
                    B00000000,
                    B00000000,
                    B00111100,
                    B11111111,
                    B01111110,
                    B00011000,
                    B00000000 },
                  { B00000000,         // Fully closed eye
                    B00000000,
                    B00000000,
                    B00000000,
                    B10000001,
                    B01111110,
                    B00000000,
                    B00000000 }
                };

static const uint8_t PROGMEM
squintImg[][8] = {
    { B00000000,
      B00000000,
      B00111100,
      B11111111,
      B11111111,
      B00111100,
      B00000000,
      B00000000 },
    { B00000000,
      B00011000,
      B00111100,
      B11111111,
      B11111111,
      B00111100,
      B00011000,
      B00000000 },
    { B00000000,
      B00000000,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B00000000,
      B00000000 },
    { B00000000,
      B00000000,
      B01111110,
      B11111111,
      B11111111,
      B01111110,
      B00000000,
      B00000000 }
};

uint8_t blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }; // Blink bitmap sequence
uint8_t closeIndex[] = { 1, 2, 3, 4 }; // Close bitmap sequence
uint8_t openIndex[] = { 3, 2, 1 }; // Open bitmap sequence
uint8_t squintIndex[] = { 3, 3 }; // squint bitmap sequence

Elwin::Elwin()
{
    motors[0].in1 = 23; motors[0].in2 = 25; motors[0].enable = 5;
    motors[1].in1 = 27; motors[1].in2 = 29; motors[1].enable = 6;
}

Elwin::~Elwin()
{

}

int Elwin::init()
{
    // Seed random number generator from an unused analog input:
    randomSeed(analogRead(A0));

    eyes.set_ss_pin(SPI_SS_PIN);
    eyes.init();

    eyes_state = ST_CENTER;
    counter=0;
    idx=0;
    eyeX=3;
    eyeY=3;
    eye_id = EYE_BOTH;

    action_state = ACT_LOOKING;
    act_counter = 0;
    blink_countdown = 200;
    pupil_countdown = 100;
    newX = 3; newY = 3;

    for(int i=0; i<MOTORS_COUNT; i++) {
        pinMode(motors[i].in1, OUTPUT);
        pinMode(motors[i].in2, OUTPUT);
    }

    move_speed = DEFAULT_SPEED;
    move_state = MOVE_STOP;
    move_stop();


#if defined(USE_SERVO)
    head_yaw = HEAD_YAW_CENTER;
    head_roll = HEAD_ROLL_CENTER;
    head_pitch = HEAD_PITCH_CENTER;

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);

    servo[0].attach(2);
    servo[1].attach(3);
    servo[2].attach(4);

    servo[0].write(head_yaw);
    delay(100);
    servo[1].write(head_roll);
    delay(100);
    servo[2].write(head_pitch);
    delay(100);

    new_yaw = HEAD_YAW_CENTER;
    new_roll = HEAD_ROLL_CENTER;
    new_pitch = HEAD_PITCH_CENTER;
    head_countdown = 1500;
    head_action_countdown = 7;
#endif //#if defined(USE_SERVO)

    return 0;
}

int Elwin::make()
{
#if 1
    int c;
    if(Serial.available()) {
        c = Serial.read();
        if(c == 'w') {
            action_state = ACT_MOVE;
            move_state = MOVE_FORWARD;
            eyes_state = ST_UP;
            eyeX--;
        }
        else if(c == 's') {
            action_state = ACT_MOVE;
            move_state = MOVE_BACWARD;
            eyes_state = ST_DOWN;
            eyeX++;
        }
        else if(c == 'a') {
            action_state = ACT_MOVE;
            move_state = MOVE_LEFT;
            eyes_state = ST_LEFT;
            eyeY--;
        }
        else if(c == 'd') {
            action_state = ACT_MOVE;
            move_state = MOVE_RIGHT;
            eyes_state = ST_RIGHT;
            eyeY++;
        }
        else if(c == ' ') {
            action_state = ACT_MOVE;
            move_state = MOVE_STOP;
        }
        else if(c == 'c') {
            eyeX = 3; eyeY = 3;
            eyes_state = ST_CENTER;
        }
        else if(c == 'h') {
            eyes_state = ST_CLOSE;
            eye_id = EYE_LEFT;
        }
        else if(c == 'j') {
            eyes_state = ST_OPEN;
            eye_id = EYE_LEFT;
        }
        else if(c == 'k') {
            eyes_state = ST_CLOSE;
            eye_id = EYE_RIGHT;
        }
        else if(c == 'l') {
            eyes_state = ST_OPEN;
            eye_id = EYE_RIGHT;
        }
        else if(c == '0') {
            action_state = ACT_SLEEP;
        }
        else if(c == '1') {
            action_state = ACT_LOOKING;
        }
        else if(c == '2') {
            action_state = ACT_SQUINT;
        }
        else if(c == '3') {
            action_state = ACT_MODEST;
        }
        else {
            eyes_state = ST_BLINK;
            eye_id = EYE_BOTH;
        }
    }

    check_pupil_position();

    if(eyes_state == ST_NONE) {
        eyes.writeDisplay();
    }
    else if(eyes_state < ST_CLOSE) {
        eyes.drawEye(EYE_BOTH);
        eyes.drawRect(EYE_BOTH, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);
        eyes.writeDisplay();
    }
    else if(eyes_state == ST_CLOSE) {
        //make_eye_sequence(eye_id, BLINK_BITMAP, CLOSE_SEQUENCE);
        make_blink_bitmap_sequence(eye_id, closeIndex, sizeof(closeIndex));
    }
    else if(eyes_state == ST_OPEN) {
        //make_eye_sequence(eye_id, BLINK_BITMAP, OPEN_SEQUENCE);
        make_blink_bitmap_sequence(eye_id, openIndex, sizeof(openIndex));
    }
    else if(eyes_state == ST_BLINK) {
        //make_eye_sequence(eye_id, BLINK_BITMAP, BLINK_SEQUENCE);
        make_blink_bitmap_sequence(eye_id, blinkIndex, sizeof(blinkIndex));
    }
    else  if(eyes_state == ST_SQUINT) {
        //make_blink_bitmap_sequence(eye_id, squintIndex, sizeof(squintIndex));
        make_bitmap_sequence(eye_id, squintImg, squintIndex, sizeof(squintIndex));
    }

    //delay(100);
#endif

    return 0;
}

int Elwin::make_action()
{
    switch (action_state) {
    case ACT_LOOKING:
        if(--blink_countdown <= 0) {
            blink_countdown = random(100, 600);
            eye_id = EYE_BOTH;
            eyes_state = ST_BLINK;
            Serial.println(F("blink"));
            Serial.println(blink_countdown);
        }
        if(--pupil_countdown <= 0) {
            pupil_countdown = random(500, 2000);
            newX = random(7); newY = random(7);
            //int dX   = newX - 3;  int dY   = newY - 3;
            Serial.println(F("pupil"));
            Serial.print(newX); Serial.print(F(" ")); Serial.println(newY);
        }

        if(--act_counter  <= 0) {
            act_counter = 100;
            if(newX>eyeX) {
                eyeX++;
            }
            else if(newX<eyeX) {
                eyeX--;
            }
            if(newY>eyeY) {
                eyeY++;
            }
            else if(newY<eyeY) {
                eyeY--;
            }
        }

#if defined(USE_SERVO)
        if(--head_countdown <= 0) {
            head_countdown = random(700, 1200);

            if( random(100) < 30 &&
                    head_yaw == HEAD_YAW_CENTER &&
                    head_roll == HEAD_ROLL_CENTER &&
                    head_pitch == HEAD_PITCH_CENTER ) {
                action_state = ACT_SQUINT;
                act_counter = 200;
            }

            if( random(100) < 30 ) {
                new_yaw = HEAD_YAW_CENTER;
                new_roll = HEAD_ROLL_CENTER;
                new_pitch = HEAD_PITCH_CENTER;
            }
            else {
                if( random(100) < 30 ) {
                    new_yaw = HEAD_YAW_CENTER - random(40);
                }
                else if(random(100) < 30) {
                    new_yaw = HEAD_YAW_CENTER + random(40);
                }
                if( random(100) < 40 ) {
                    new_roll = HEAD_ROLL_CENTER - random(45);
                }
                else if(random(100) < 40) {
                    new_roll = HEAD_ROLL_CENTER + random(45);
                }
                if( random(100) < 50 ) {
                    new_pitch = HEAD_PITCH_CENTER - random(35);
                    newX = 3;
                    newY = 5;
                }
                else {
                    new_pitch = HEAD_PITCH_CENTER + random(35);
                    newX = 3;
                    newY = 1;
                }
            }

            Serial.println(F("head"));
            Serial.print(F("Y: "));
            Serial.print(head_yaw);
            Serial.print(F(" R: "));
            Serial.print(head_roll);
            Serial.print(F(" P: "));
            Serial.println(head_pitch);
            Serial.print(F("Y2: "));
            Serial.print(new_yaw);
            Serial.print(F(" R2: "));
            Serial.print(new_roll);
            Serial.print(F(" P2: "));
            Serial.println(new_pitch);
        }
#endif //#if defined(USE_SERVO)
        break;
    case ACT_SQUINT:
        eye_id = EYE_BOTH;
        eyes_state = ST_SQUINT;
        eyeX=3;
        eyeY=3;

        if(--act_counter  <= 0) {
            act_counter = 100;
            Serial.println(F("squint"));

            if( random(100) < 40 ) {
                action_state = ACT_LOOKING;
                blink_countdown = 0;
            }
        }

        break;
    case ACT_MOVE:
        switch (move_state) {
        case MOVE_FORWARD:
            move_forward(move_speed);
            break;
        case MOVE_BACWARD:
            move_backward(move_speed);
            break;
        case MOVE_LEFT:
            move_left(move_speed);
            break;
        case MOVE_RIGHT:
            move_right(move_speed);
            break;
        case MOVE_STOP:
        default:
            move_stop();
            break;
        }

        if(move_state == MOVE_STOP) {
            if(--act_counter  <= 0) {
                act_counter = 100;
                action_state = ACT_LOOKING;
                blink_countdown = 200;
            }
        }

        if(--blink_countdown <= 0) {
            blink_countdown = random(400, 800);
            eye_id = EYE_BOTH;
            eyes_state = ST_BLINK;
        }

#if defined(USE_SERVO)
        new_yaw = HEAD_YAW_CENTER;
        new_roll = HEAD_ROLL_CENTER;
        new_pitch = HEAD_PITCH_CENTER;
#endif //#if defined(USE_SERVO)

        break;
    default:
        action_state = ACT_LOOKING;
        break;
    }

#if defined(USE_SERVO)
    if(--head_action_countdown <= 0) {
        head_action_countdown = 6;
        if(new_yaw > head_yaw) {
            head_yaw++;
        }
        else if(new_yaw < head_yaw) {
            head_yaw--;
        }
        if(new_roll > head_roll) {
            head_roll++;
        }
        else if(new_roll < head_roll) {
            head_roll--;
        }
        if(new_pitch > head_pitch) {
            head_pitch++;
        }
        else if(new_pitch < head_pitch) {
            head_pitch--;
        }

        servo[0].write(head_yaw);
        servo[1].write(head_roll);
        servo[2].write(head_pitch);
    }
#endif //#if defined(USE_SERVO)

    make();

    return 0;
}

void Elwin::make_eye_sequence(int eye_id, int bitmap_id, int sequence_id)
{
    switch(bitmap_id) {
    case SQUINT_BITMAP:
        switch (sequence_id) {
        case SQUINT_SEQUENCE:
        default:
            make_bitmap_sequence(eye_id, squintImg, squintIndex, sizeof(squintIndex));
            break;
        }
        break;
    case BLINK_BITMAP:
    default:
        switch (sequence_id) {
        case CLOSE_SEQUENCE:
            make_blink_bitmap_sequence(eye_id, closeIndex, sizeof(closeIndex));
            break;
        case OPEN_SEQUENCE:
            make_blink_bitmap_sequence(eye_id, openIndex, sizeof(openIndex));
            break;
        case BLINK_SEQUENCE:
        default:
            make_blink_bitmap_sequence(eye_id, blinkIndex, sizeof(blinkIndex));
            break;
        }

        break;
    }
}

void Elwin::make_blink_bitmap_sequence(int _eye_id, uint8_t* seq, size_t seq_size)
{
    make_bitmap_sequence(_eye_id, blinkImg, seq, seq_size);
}

void Elwin::make_bitmap_sequence(int _eye_id, const uint8_t bitmap[][8], uint8_t *seq, size_t seq_size)
{
    eye_id = _eye_id;
    eyes.drawBitmap(eye_id, 0, 0, bitmap[seq[idx]], 8, 8, LED_ON);
    eyes.drawRect(eye_id, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);

    eyes.writeDisplay();
    delay(1);
    counter++;
    if(counter>5) {
        eyes.clear(eye_id);
        counter=0;
        idx++;
        if(idx >= seq_size) {
            idx=0;
            if(eyes_state == ST_BLINK || eyes_state == ST_OPEN) {
                eyes_state = ST_CENTER;
            }
            else if(eyes_state == ST_CLOSE) {
                eyes_state = ST_NONE;
                eyes.drawBitmap(eye_id, 0, 0, bitmap[seq[seq_size-1]], 8, 8, LED_ON);
            }
        }
    }
}

void Elwin::check_pupil_position()
{
    if(eyeX < 1) eyeX=1;
    if(eyeY < 1) eyeY=1;
    if(eyeX > 5) eyeX=5;
    if(eyeY > 5) eyeY=5;

    if(eyeX == 1 && eyeY == 1) eyeX = 2;
    if(eyeX == 5 && eyeY == 5) eyeX = 4;
    if(eyeX == 1 && eyeY == 5) eyeX = 2;
    if(eyeX == 5 && eyeY == 1) eyeY = 2;
}

void Elwin::motor_drive(int motor_id, int dir, int pwm)
{
    if(motor_id < 0 || motor_id >= MOTORS_COUNT)
        return;

    if(dir == FORWARD) {
        digitalWrite(motors[motor_id].in1, HIGH);
        digitalWrite(motors[motor_id].in2, LOW);
    }
    else {
        digitalWrite(motors[motor_id].in1, LOW);
        digitalWrite(motors[motor_id].in2, HIGH);
    }
    analogWrite(motors[motor_id].enable, pwm);
}

void Elwin::move_forward(int pwm)
{
    for(int i=0; i<MOTORS_COUNT; i++) {
        motor_drive(i, FORWARD, pwm);
    }
}

void Elwin::move_backward(int pwm)
{
    for(int i=0; i<MOTORS_COUNT; i++) {
        motor_drive(i, BACKWARD, pwm);
    }
}

void Elwin::move_left(int pwm)
{
    motor_drive(0, BACKWARD, pwm);
    motor_drive(1, FORWARD, pwm);
}

void Elwin::move_right(int pwm)
{
    motor_drive(0, FORWARD, pwm);
    motor_drive(1, BACKWARD, pwm);
}

void Elwin::move_stop()
{
    for(int i=0; i<MOTORS_COUNT; i++) {
        motor_drive(i, FORWARD, 0);
    }
}
