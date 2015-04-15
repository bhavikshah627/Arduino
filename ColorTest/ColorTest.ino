#include <Wire.h>
#include "Adafruit_TCS34725.h"

/*
tthese values are with lights off in Lab 3:

B:
c: 1100
W:
c: 4020
G:
c: 2400
*/
//color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//for Adafruit Color Sensor
int bl = 990, wh = 3800, gr = 1700;



void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  delay(100);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
}

void loop()
{
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);  // turn off LED

  char buf[8];
  sprintf(buf, "c: %d \t",clear);
  Serial.print(buf);
  if( clear < 2000 && clear > 1700 && green > 620)
    Serial.print("GREEN");
  Serial.println();
}
