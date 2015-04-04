//#include <MotorLib.h>

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 145;
const int GRN_L = 170; //change later
const int WHT_L = 265;
const int BLK_M = 85;       //middles need to be changed
const int GRN_M = 165;
const int WHT_M = 270;
const int BLK_R = 115;
const int GRN_R = 145; //change later
const int WHT_R = 185;

const int TRS_L_1 = (BLK_L + GRN_L) / 2;
const int TRS_L_2 = (GRN_L + WHT_L) / 2;
const int TRS_M_1 = (BLK_M + GRN_M) / 2;
const int TRS_M_2 = (GRN_M + WHT_M) / 2;
const int TRS_R_1 = (BLK_R + GRN_R) / 2;
const int TRS_R_2 = (GRN_R + WHT_R) / 2;

int LFT, MID, RHT;


int getLFT(){return analogRead(analogInPinOne);}
int getMID(){return analogRead(analogInPinTwo);}
int getRHT(){return analogRead(analogInPinThree);}

void setup()
{
  Serial.begin(9600);
  initMotors();
}

void loop() 
{
  int lftval = getLFT();
  int midval = getMID();
  int rhtval = getRHT();
  //exception for left green
  /*
  -
  -
  -
  --------
  -
  -
  -
  
  */
  if(rhtval < TRS_R_1)
    setbothSpeeds(60, -40);
  else if(lftval > TRS_L_2 && rhtval > TRS_R_2)
    setbothSpeeds(40, 40);
  else if(lftval < TRS_L_1 && rhtval >TRS_R_2)
    setbothSpeeds(-40,60);
  
  char buf[10];
  sprintf(buf, "%d %d", lftval, rhtval);
  Serial.println(buf);
}
