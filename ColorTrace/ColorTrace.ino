//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int B_L = 1000;
const int G_L = 1600;
const int W_L = 3400;

const int B_M = 1200;
const int W_M = 4000;

const int B_R = 1000;
const int G_R = 1600;
const int W_R = 3400;

const int T_L_1 = (B_L + G_L) / 2;
const int T_L_2 = (G_L + W_L) / 2;
const int T_M = (B_M + W_M) / 2;
const int T_R_1 = (B_R + G_R) / 2;
const int T_R_2 = (G_R + W_R) / 2;


int muxports[] = { 1, 3};
//left, middle, right;
const int numports = sizeof(muxports) / sizeof(int);
uint16_t clear[2], red[2], blue[2], green[2], mclear, mred, mblue, mgreen;
Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void getMidC() 
{

    if (mux(0)) Serial.println("Yo");
    tcsleft.setInterrupt(false);      // turn on LED  

  delay(60);  // takes 50ms to read
    tcsleft.getRawData(&mred, &mgreen, &mblue, &mclear);
    tcsleft.setInterrupt(true);  // turn off LED

}


void getSideC() 
{
  int index = 0;

  for (index = 0; index < numports; index++) {
    if (mux(muxports[index])) Serial.println("Yo");
    tcsleft.setInterrupt(false);      // turn on LED
  }

  delay(60);  // takes 50ms to read

  for ( index = 0; index < numports; index++)
  {
    mux(muxports[index]);
    tcsleft.getRawData(&red[index], &green[index], &blue[index], &clear[index]);
    tcsleft.setInterrupt(true);  // turn off LED
  }
}



const int VEER_NONE = 0;
const int VEER_LEFT  = 1;
const int VEER_RIGHT  = 2;
const int VEER_STRAIGHT = 3;

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
  
    if (mux(0)) Serial.println("ERROR MUX");
    Serial.print("Init sensor ");
    Serial.println(0);
    if (tcsleft.begin()) {
      Serial.println("Found sensor");
    }
    else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  
  
  initMotors();
  delay(1000);
}

void loop()
{
  static int state = VEER_NONE;
  const int  fpwr = 30;
  const int  bpwr = -20;

  //0 elment is left, 1 is middle, 2 is right
  getSideC();
  getMidC();
  char buf[40];
  sprintf(buf, "%d, %d, %d", clear[0], mclear, clear[1]); 
  Serial.println(buf);
  if (clear[1] < T_R_1  && state != VEER_RIGHT )
  { //right sees black
    getMidC();
    if (mclear < 1500)
    {
      specDist(1, fpwr);
      specDistTurn(5, fpwr, false);
      }
    else
    {
      Serial.println("RIGHT");
      setbothSpeeds(fpwr, bpwr);
      state = VEER_RIGHT;
    }
  }
  else if (clear[0] < T_L_1  && clear[1] > T_R_2 && state != VEER_LEFT) { // left sees black and right sees white
    setbothSpeeds(bpwr, fpwr);
    state = VEER_LEFT;
  }
  else if (clear[0] > T_L_2 && clear[1] > T_R_2 && state != VEER_STRAIGHT) { //left and right sees white
    setbothSpeeds(fpwr, fpwr);
    state = VEER_STRAIGHT;
  }
}
