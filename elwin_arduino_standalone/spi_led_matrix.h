//
// control LED Matrix 8x8 over SPI
//
// http://robocraft.ru
//

#ifndef SPI_LED_MATRIX_H
#define SPI_LED_MATRIX_H

#include <inttypes.h>
#include <SPI.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

enum {
    LED_OFF = LOW,
    LED_ON = HIGH
};

enum {
    EYE_LEFT = 0,
    EYE_RIGHT = 1,
    EYE_BOTH = 2
};

#define LEDS_MATRIX_L (8)
#define LEDS_MATRIX_SIZE (16) //8x8

class SPI_LedMatrix
{
public:
    SPI_LedMatrix(int ss_pin);
    ~SPI_LedMatrix();

    void clear(int mat_id);
    void drawPixel(int mat_id, int x, int y, int val);
    void drawRect(int mat_id, int x_top, int y_top, int x_bottom, int y_bottom, int val);

    void writeDisplay();
    void drawEye(int mat_id);

    void drawBitmap(int mat_id, int16_t x, int16_t y,
                      const uint8_t *bitmap, int16_t w, int16_t h,
                      int val);

private:
    int ss_pin;

    uint8_t buf[8];
    uint8_t buf2[8];
};

#endif // #ifndef SPI_LED_MATRIX_H
