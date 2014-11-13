//
// control LED Matrix 8x8 over SPI
//
// http://robocraft.ru
//

#include "spi_led_matrix.h"

SPI_LedMatrix::SPI_LedMatrix(int _ss_pin):
    ss_pin(_ss_pin)
{
    SPI.begin();
    pinMode(ss_pin, OUTPUT);
    memset(buf, 0, sizeof(buf));
    memset(buf2, 0, sizeof(buf2));
}

SPI_LedMatrix::~SPI_LedMatrix()
{

}

void SPI_LedMatrix::clear(int mat_id)
{
    for(int i=0; i<LEDS_MATRIX_L; i++) {
        if(mat_id == EYE_LEFT) {
            buf[i] = 0;
        }
        else if(mat_id == EYE_RIGHT) {
            buf2[i] = 0;
        }
        else {
            buf[i] = 0;
            buf2[i] = 0;
        }
    }
}

void SPI_LedMatrix::drawPixel(int mat_id, int x, int y, int val)
{
    if ((y < 0) || (y >= 8)) return;
    if ((x < 0) || (x >= 16)) return;

    if(mat_id == EYE_LEFT) {
        if (val) {
            buf[y] |= 1 << x;
        } else {
            buf[y] &= ~(1 << x);
        }
    }
    else if(mat_id == EYE_RIGHT) {
        if (val) {
            buf2[y] |= 1 << x;
        } else {
            buf2[y] &= ~(1 << x);
        }
    }
    else {
        if (val) {
            buf[y] |= 1 << x;
            buf2[y] |= 1 << x;
        } else {
            buf[y] &= ~(1 << x);
            buf2[y] &= ~(1 << x);
        }
    }
}

void SPI_LedMatrix::drawRect(int mat_id, int x_top, int y_top, int x_bottom, int y_bottom, int val)
{
    for(int y=y_top; y<y_bottom; y++) {
        for(int x=x_top; x<x_bottom; x++) {
            drawPixel(mat_id, x, y, val);
        }
    }
}

void SPI_LedMatrix::writeDisplay()
{
    for (uint8_t i=0; i<8; i++) {
        digitalWrite(ss_pin, LOW);
        //Serial.print( i );
        //Serial.print( " " );
        //Serial.println( (uint8_t)(1 << i), BIN );
        //SPI.transfer(B1000000);
        //SPI.transfer(B1000000);
        SPI.transfer( (uint8_t)(1 << i) );
        SPI.transfer( (uint8_t)(buf2[i] & 0xFF) );

        SPI.transfer( (uint8_t)(1 << i) );
        SPI.transfer( (uint8_t)(buf[i] & 0xFF) );

        //SPI.transfer(buf[i] >> 8);
        digitalWrite(ss_pin, HIGH);
        delay(1);
    }
}

void SPI_LedMatrix::drawEye(int mat_id)
{
    drawRect(mat_id, 0, 0, 8, 8, LED_ON);

    drawPixel(mat_id, 0, 0, LED_OFF);
    drawPixel(mat_id, 1, 0, LED_OFF);
    drawPixel(mat_id, 0, 1, LED_OFF);

    drawPixel(mat_id, 0, 6, LED_OFF);
    drawPixel(mat_id, 1, 7, LED_OFF);
    drawPixel(mat_id, 0, 7, LED_OFF);

    drawPixel(mat_id, 6, 0, LED_OFF);
    drawPixel(mat_id, 7, 0, LED_OFF);
    drawPixel(mat_id, 7, 1, LED_OFF);

    drawPixel(mat_id, 6, 7, LED_OFF);
    drawPixel(mat_id, 7, 7, LED_OFF);
    drawPixel(mat_id, 7, 6, LED_OFF);

    //drawRect(0, 3, 3, 5, 5, LED_OFF);
}

void SPI_LedMatrix::drawBitmap(int mat_id, int16_t x, int16_t y,
                               const uint8_t *bitmap, int16_t w, int16_t h,
                               int val)
{
    int16_t i, j;
    int16_t counter=0;
    for(j=0; j<h; j++) {
        uint8_t bb = pgm_read_byte(bitmap + counter);
        //Serial.println( bb, BIN );
        counter++;
        for(i=0; i<w; i++) {
            if(bb & (1<<i)) {
                drawPixel(mat_id, x+j, y+i, val);
            }
        }
    }
}
