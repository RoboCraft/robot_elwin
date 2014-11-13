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
  blinkImg[][8] = {    // Eye animation frames
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
    B00000000 } };

uint8_t blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }; // Blink bitmap sequence
uint8_t closeIndex[] = { 1, 2, 3, 4 }; // Close bitmap sequence
uint8_t openIndex[] = { 3, 2, 1 }; // Open bitmap sequence

Elwin::Elwin()
{
}

Elwin::~Elwin()
{

}

int Elwin::init()
{
    eyes.set_ss_pin(SPI_SS_PIN);
    eyes.init();

    eyes_state = ST_CENTER;
    counter=0;
    idx=0;
    eyeX=3;
    eyeY=3;
    eye_id = EYE_BOTH;

    return 0;
}

int Elwin::make()
{
#if 1
    int c;
    if(Serial.available()) {
        c = Serial.read();
        if(c == 'w') {
            eyes_state = ST_UP;
            eyeX--;
        }
        else if(c == 's') {
            eyes_state = ST_DOWN;
            eyeX++;
        }
        else if(c == 'a') {
            eyes_state = ST_LEFT;
            eyeY--;
        }
        else if(c == 'd') {
            eyes_state = ST_RIGHT;
            eyeY++;
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
        make_eye_sequence(eye_id, BLINK_BITMAP, CLOSE_SEQUENCE);
    }
    else if(eyes_state == ST_OPEN) {
        make_eye_sequence(eye_id, BLINK_BITMAP, OPEN_SEQUENCE);
    }
    else if(eyes_state == ST_BLINK) {
        make_eye_sequence(eye_id, BLINK_BITMAP, BLINK_SEQUENCE);
    }

    //delay(100);
#endif

    return 0;
}

void Elwin::make_eye_sequence(int eye_id, int bitmap_id, int sequence_id)
{
    switch(bitmap_id) {
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
    eye_id = _eye_id;
    eyes.drawBitmap(eye_id, 0, 0, blinkImg[seq[idx]], 8, 8, LED_ON);
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
                eyes.drawBitmap(eye_id, 0, 0, blinkImg[seq[seq_size-1]], 8, 8, LED_ON);
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
}
