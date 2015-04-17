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
Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


int muxports[] = { 1, 0, 3};
//left, middle, right;
const int numports = sizeof(muxports) / sizeof(int);
uint16_t clear[3], red[3], blue[3], green[3];

//for Adafruit Color Sensor
int bl = 990, wh = 3800, gr = 1700;

int mux(byte channel)
{
  const int MUX = 0x70;
  byte controlRegister = 0x04;
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  if (channel == 0xFF) {
    Wire.write(0x00); //deselect all channels
  }
  else {
    Wire.write(controlRegister); //set to selected channel
  }
  return Wire.endTransmission();
}

//mux 0 3
void setup()
{
  Serial.begin(9600);

  Wire.begin();
  //TWBR=12;
  Serial.println("Starting...");
  Serial.println(numports);
  delay(100);

  for (int i = 0; i < numports; i++) {
    if (mux(muxports[i])) Serial.println("ERROR MUX");
    Serial.print("Init sensor ");
    Serial.println(muxports[i]);
    if (tcsleft.begin()) {
      Serial.println("Found sensor");
    }
    else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  }
  delay(2000);
}

void basiccall() {
  int index = 0;
  //uint16_t clear, red, green, blue;

  for (index = 0; index < numports; index++) {

    if (mux(muxports[index])) Serial.println("Yo");

    tcsleft.setInterrupt(false);      // turn on LED
    delay(60);  // takes 50ms to read
    tcsleft.getRawData(&red[index], &green[index], &blue[index], &clear[index]);
    tcsleft.setInterrupt(true);  // turn off LED


    char buf[40];
    sprintf(buf, "%d - %d %d %d %d", muxports[index], clear[index], red[index], green[index], blue[index] );
    Serial.println(buf);
  }
}


void loop()
{
  advancedcall();

}



void advancedcall() {
  int index = 0;

  for (index = 0; index < numports; index++) {
    if (mux(muxports[index])) Serial.println("Yo");
    tcsleft.setInterrupt(false);      // turn on LED
  }

  delay(30);  // takes 50ms to read

  for ( index = 0; index < numports; index++)
  {
    mux(muxports[index]);
    tcsleft.getRawData(&red[index], &green[index], &blue[index], &clear[index]);
    tcsleft.setInterrupt(true);  // turn off LED

  }
   static char buf[40];
  sprintf(buf, " %d %d %d",  clear[0] , clear[1], clear[2]);
  Serial.println(buf);

}

