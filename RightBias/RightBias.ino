//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 140;
const int GRN_L = 280; 
const int WHT_L = 345;

const int BLK_M = 395;       
const int GRN_M = 486;
const int WHT_M = 555;

const int BLK_R = 307;
const int GRN_R = 377; 
const int WHT_R = 414;

//const int BLK_C = 
//const int GRN_C = 3110
//const int WHT_C = 4510;
//
//const int BLK_G = 
//const int GRN_G = 
//const int WHT_G = 

//const int TRS_L_1 = (BLK_L + GRN_L) / 2;
//const int TRS_L_2 = (GRN_L + WHT_L) / 2;
//const int TRS_M_1 = (BLK_M + GRN_M) / 2;
//const int TRS_M_2 = (GRN_M + WHT_M) / 2;
//const int TRS_R_1 = (BLK_R + GRN_R) / 2;
//const int TRS_R_2 = (GRN_R + WHT_R) / 2;

const int TRS_L = (BLK_L + WHT_L) / 2;
const int TRS_M = (BLK_M + WHT_M) / 2;
const int TRS_R = (BLK_R + WHT_R) / 2;
//const int TRS_C_1 = (BLK_C + GRN_C) / 2;
//const int TRS_C_2 = (GRN_C + WHT_C) / 2;

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
  pinMode(11, OUTPUT);
  tcs.begin(); 
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

  digitalWrite(11, HIGH);
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read 
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);  // turn off LED
  digitalWrite(11, LOW);
    
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
  sprintf(buf, "%d %d %d %d", lftval, midval, rhtval, clear);
  Serial.println(buf);
}
