#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int B_L = 1110;
const int G_L = 2014;
const int W_L = 3000;

const int B_M = 946;
const int W_M = 3558;

const int B_R = 1180;
const int G_R = 2165;
const int W_R = 3805;

const int GREEN_GRD_L = 600;
const int GREEN_GRD_R = 600;

const int T_L_1 = (B_L + G_L) / 2;
const int T_L_2 = (G_L + W_L) / 2;
const int T_M = (B_M + W_M) / 2;
const int T_R_1 = (B_R + G_R) / 2;
const int T_R_2 = (G_R + W_R) / 2;

int muxports[] = {1, 3};
//left, middle, right;
const int numports = sizeof(muxports) / sizeof(int);
uint16_t clear[2], red[2], blue[2], green[2], mclear, mred, mblue, mgreen;
Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void getMidC()
{
  if (mux(0)) Serial.println("Yo");
  tcsleft.getRawDataEx(&mred, &mgreen, &mblue, &mclear);
}

void getSideC()
{
  int index = 0;
  for (index = 0; index < numports; index++)
  {
    if (mux(muxports[index])) Serial.println("Yo");
    tcsleft.getRawDataEx(&red[index], &green[index], &blue[index], &clear[index]);
  }
}

int mux(byte channel)
{
  const int MUX = 0x70;
  byte controlRegister = 0x04;
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  if (channel == 0xFF)
  {
    Wire.write(0x00); //deselect all channels
  }
  else
  {
    Wire.write(controlRegister); //set to selected channel
  }
  return Wire.endTransmission();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  TWBR = 12;
  for (int i = 0; i < numports; i++)
  {
    if (mux(muxports[i])) Serial.println("ERROR MUX");
    Serial.print("Init sensor ");
    Serial.println(muxports[i]);
    if (tcsleft.begin())
    {
      Serial.println("Found sensor");
      tcsleft.setInterrupt(false);
    }
    else 
    {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  }

  delay(60);
  initMotors();
}

//green = 726, 763, 383

void loop() {
  getSideC();
  getMidC();
  char buf[40];
//  sprintf(buf, "%d < %d < %d, %d < %d", T_L_1, clear[0], T_L_2, GREEN_GRD_L, green[0]);
sprintf(buf, "blue: %d, green: %d, red: %d", blue[1], green[1], red[1]);
//  sprintf(buf, "clear: %d", clear[0]);
  Serial.print(buf);

 if (blue[1] < 400 && green[1] > 600)
  {
    Serial.print(" GREEN");
  }
  
  Serial.println();

}
