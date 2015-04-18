//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int B_L = 1110;
//const int G_L = 2014;
const int W_L = 3485;

const int B_M = 946;
const int W_M = 3558;

const int B_R = 1180;
//const int G_R = 2165;
const int W_R = 3805;;

const int T_L = (B_L + W_L) / 2;
//const int T_L_2 = (G_L + W_L) / 2;
const int T_M = (B_M + W_M) / 2;
const int T_R = (B_R + W_R) / 2;
//const int T_R_2 = (G_R + W_R) / 2;

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
    else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  }

  if (mux(0)) Serial.println("ERROR MUX");
  Serial.print("Init sensor ");
  Serial.println(0);
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

  delay(60);
  initMotors();
  delay(1000);
}

void loop()
{
  static int state = VEER_NONE;
  const int  fpwr = 127;
  const int  bpwr = -35;
  getSideC();
  getMidC();
  char buf[40];
  sprintf(buf, "%d  %d", clear[0], clear[1]);
  Serial.println(buf);
  
  if (blue[0] < 300 && green[0] > 450)
  {//left green --only sometimes work
    specDist(10, 30);
    while(mclear<T_M)
    {
      getMidC();
      setbothSpeeds(-30, 30);
    }
    while(mclear>T_M)
    {  
      setbothSpeeds(-30, 30);
      getMidC();
    }
  }
  else if (clear[1] < T_R)
  { //right sees black
    if (mclear < T_M)
    {//middle also sees black
      specDist(1, fpwr);
      specDistTurn(5, fpwr, false);
    }
    else if (state != VEER_RIGHT)
    {
      Serial.println("RIGHT");
      setbothSpeeds(fpwr, bpwr);
      state = VEER_RIGHT;
    }
  }
  else if (clear[0] < T_L  && clear[1] > T_R && state != VEER_LEFT)
  { // left sees black and right sees white
    setbothSpeeds(bpwr, fpwr);
    state = VEER_LEFT;
  }
  else if (clear[0] > T_L  && clear[1] > T_R)
  { // left sees white and right sees white
    setbothSpeeds(fpwr, fpwr);
    state = VEER_STRAIGHT;
  }
  /*else
  {
    int timer = millis();
    while (clear[0] > T_L_2 && clear[1] > T_R_2) //left and right sees white //wobble wobble wobble
    {
      getSideC();
      getMidC();
      if (millis() < timer + 1000 && state != VEER_STRAIGHT) // go forward for 1 second
      {
        setbothSpeeds(fpwr, fpwr);
        state = VEER_STRAIGHT;
      }
      else
      {
        //     current time - starting time - 1sec foward time = time passed since 1 second forward
        //     time passed / 1000 = how many seconds passed
        //     seconds passed mod 2 = odd or even amnt. of seconds passed
        if ((((millis() - timer - 1000) / 1000) % 2) == 1) // intervals of 1 millisecond wobble
        {
          setbothSpeeds(fpwr, 0);
        }
        else if ((((millis() - timer - 1000) / 1000) % 2) == 0)
        {
          setbothSpeeds(0, fpwr);
        }
      }
      delay(20);
    }
  }*/
}
