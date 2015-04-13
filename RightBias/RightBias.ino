//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 140;
const int GRN_L = 205; 
const int WHT_L = 258;

const int BLK_M = 395;       
const int GRN_M = 448;
const int WHT_M = 536;

const int BLK_R = 307;
const int GRN_R = 401; 
const int WHT_R = 456;

//const int TRS_L_1 = (BLK_L + GRN_L) / 2;
//const int TRS_L_2 = (GRN_L + WHT_L) / 2;
//const int TRS_M_1 = (BLK_M + GRN_M) / 2;
//const int TRS_M_2 = (GRN_M + WHT_M) / 2;
//const int TRS_R_1 = (BLK_R + GRN_R) / 2;
//const int TRS_R_2 = (GRN_R + WHT_R) / 2;

const int TRS_L = (BLK_L + WHT_L) / 2;
const int TRS_M = (BLK_M + WHT_M) / 2;
const int TRS_R = (BLK_R + WHT_R) / 2;

int LFT, MID, RHT;

const int VEER_NONE = 0;
const int VEER_LEFT  = 1;
const int VEER_RIGHT  = 2;
const int VEER_STRAIGHT = 3;

int getLFT(){return analogRead(analogInPinOne);}
int getMID(){return analogRead(analogInPinTwo);}
int getRHT(){return analogRead(analogInPinThree);}

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
  Serial.begin(9600);
  initMotors();
  delay(1000);
}

void loop() 
{
  static int state = VEER_NONE;
  int lftval = getLFT();
  int midval = getMID();
  int rhtval = getRHT();
  const int  fpwr = 40;
  const int  bpwr = -35;
  
  //40 , -30
  
  //getting color data <-=

//  uint16_t clear, red, green, blue;
//  tcs.setInterrupt(false);      // turn on LED
//  delay(60);  // takes 50ms to read 
//  tcs.getRawData(&red, &green, &blue, &clear);
//  tcs.setInterrupt(true);  // turn off LED
  
  //exception for left green
  // if(lftval > TRS_R_1 && lftval < TRS_R_2)
  // specDistTurn(90, fpwr, false);
  if(rhtval < TRS_R && state != VEER_RIGHT )
  {  //right sees black
    if(midval<TRS_M)
    {
      specDist(1, fpwr); 
      specDistTurn(5, fpwr, false);
    }
    else
    {
    setbothSpeeds(fpwr, bpwr);
    state = VEER_RIGHT;
    }
  }
    else if(lftval < TRS_L && rhtval > TRS_R && state != VEER_LEFT){ // left sees black and right sees white
      setbothSpeeds(bpwr,fpwr);
      state = VEER_LEFT;
  }
    else if(lftval > TRS_L && rhtval > TRS_R && state != VEER_STRAIGHT){  //left and right sees white
      setbothSpeeds(fpwr, fpwr);
      state = VEER_STRAIGHT;
  }
  
  char buf[10];
  sprintf(buf, "%d %d %d", lftval, midval, rhtval);
  Serial.println(buf);
}
