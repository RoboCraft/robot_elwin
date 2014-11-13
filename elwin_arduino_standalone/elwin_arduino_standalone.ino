//
// test LED matrix 8x8 over SPI
//
// http://robocraft.ru
//

#include <SPI.h>
#include "spi_led_matrix.h"

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


// выходной каскад из двух сдвиговых регистров 74HC595
// SS на 53 pin
SPI_LedMatrix eyes(53);

void setup()
{
    Serial.begin(57600);

    Serial.println(F("[i] Start..."));
}

enum {ST_NONE=0, ST_CENTER=1, ST_UP, ST_DOWN, ST_LEFT, ST_RIGHT,
      ST_CLOSE, ST_OPEN, ST_BLINK};
int state = ST_CENTER;
int counter=0, idx=0;
int eyeX=3, eyeY=3;
int close_eye = EYE_BOTH;

void make_bitmap_sequence(uint8_t* seq, size_t seq_size)
{
    eyes.drawBitmap(close_eye, 0, 0, blinkImg[seq[idx]], 8, 8, LED_ON);
    eyes.drawRect(close_eye, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);

    eyes.writeDisplay();
    delay(1);
    counter++;
    if(counter>5) {
        eyes.clear(close_eye);
        counter=0;
        idx++;
        if(idx >= seq_size) {
            idx=0;
            if(state == ST_BLINK || state == ST_OPEN) {
                state = ST_CENTER;
            }
            else if(state == ST_CLOSE) {
                state = ST_NONE;
                eyes.drawBitmap(close_eye, 0, 0, blinkImg[seq[seq_size-1]], 8, 8, LED_ON);
            }
        }
    }
}

void loop()
{

#if 1

    int c;
    if(Serial.available()) {
        c = Serial.read();
        if(c == 'w') {
            state = ST_UP;
            eyeX--;
        }
        else if(c == 's') {
            state = ST_DOWN;
            eyeX++;
        }
        else if(c == 'a') {
            state = ST_LEFT;
            eyeY--;
        }
        else if(c == 'd') {
            state = ST_RIGHT;
            eyeY++;
        }
        else if(c == 'c') {
            eyeX = 3; eyeY = 3;
            state = ST_CENTER;
        }
        else if(c == 'h') {
            state = ST_CLOSE;
            close_eye = EYE_LEFT;
        }
        else if(c == 'j') {
            state = ST_OPEN;
            close_eye = EYE_LEFT;
        }
        else if(c == 'k') {
            state = ST_CLOSE;
            close_eye = EYE_RIGHT;
        }
        else if(c == 'l') {
            state = ST_OPEN;
            close_eye = EYE_RIGHT;
        }
        else {
            state = ST_BLINK;
            close_eye = EYE_BOTH;
        }
    }

    if(eyeX < 1) eyeX=1;
    if(eyeY < 1) eyeY=1;
    if(eyeX > 5) eyeX=5;
    if(eyeY > 5) eyeY=5;

    if(state == ST_NONE) {
        eyes.writeDisplay();
    }
    else if(state < ST_CLOSE) {
        eyes.drawEye(EYE_BOTH);
        eyes.drawRect(EYE_BOTH, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);
        eyes.writeDisplay();
    }
    else if(state == ST_CLOSE) {
        make_bitmap_sequence(closeIndex, sizeof(closeIndex));
    }
    else if(state == ST_OPEN) {
        make_bitmap_sequence(openIndex, sizeof(openIndex));
    }
    else if(state == ST_BLINK) {
        make_bitmap_sequence(blinkIndex, sizeof(blinkIndex));
    }

    //delay(100);
#endif

}
