//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

<<<<<<< HEAD
const int BLK_L = 225;
const int GRN_L = 258; 
const int WHT_L = 377;
=======
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


const int BLK_L = 110;
const int GRN_L = 160; 
const int WHT_L = 223;
>>>>>>> origin/master

const int BLK_M = 550;       
const int GRN_M = 485;
const int WHT_M = 636;

const int BLK_R = 329;
const int GRN_R = 357; 
const int WHT_R = 463;

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

void setup()
{
  Serial.begin(9600);
  initMotors();
  delay(3000);
}

void loop() 
{
  static int state = VEER_NONE;
  int lftval = getLFT();
  int midval = getMID();
  int rhtval = getRHT();
  const int  fpwr = 40;
  const int  bpwr = -30;
  //exception for left green
  
<<<<<<< HEAD
 // if(lftval > TRS_R_1 && lftval < TRS_R_2)
   // specDistTurn(90, fpwr, false);
  if(rhtval < TRS_R && state != VEER_RIGHT )
=======
  //getting color data <3
   uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  
  //<3
  
  
  if(lftval > TRS_R_1 && lftval < TRS_R_2)
    specDistTurn(90, fpwr, false);
  else if(rhtval < TRS_R_1 && state != VEER_RIGHT )
>>>>>>> origin/master
  {  //right sees black
    if(midval < TRS_M)
    {
      specDist(1, fpwr); 
      specDistTurn(10, fpwr, false);
    }
    else
    {
    setbothSpeeds(fpwr, bpwr);
    state = VEER_RIGHT;
    }
  }
  else if(lftval < TRS_L && rhtval > TRS_R && state != VEER_STRAIGHT){ //left sees black and right sees white
    setbothSpeeds(bpwr, fpwr);
    state = VEER_STRAIGHT;
  }
  else if(lftval > TRS_L && rhtval > TRS_R && state != VEER_LEFT){ //left and right sees white
    setbothSpeeds(fpwr,fpwr);
    state = VEER_STRAIGHT;
  }
//  else
//  {
//    setbothSpeeds(fpwr,fpwr);
//  }
  
  char buf[10];
  sprintf(buf, "%d %d %d", lftval, midval, rhtval);
  Serial.println(buf);
}
