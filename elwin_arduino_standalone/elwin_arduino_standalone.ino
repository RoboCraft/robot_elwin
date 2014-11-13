//
// test LED matrix 8x8 over SPI
//
// http://robocraft.ru
//

#include "definitions.h"
#include <SPI.h>
#include "spi_led_matrix.h"
#include "elwin_arduino.h"
#if defined(USE_SERVO)
#include <Servo.h>
#endif

Elwin elwin;

void setup()
{
    Serial.begin(57600);

    Serial.println(F("[i] init..."));

    elwin.init();

    Serial.println(F("[i] Start..."));
}


void loop()
{
    elwin.make();
}
