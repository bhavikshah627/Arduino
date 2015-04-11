//#include <MotorLib.h>

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 130;
const int GRN_L = 200; 
const int WHT_L = 275;

const int BLK_M = 328;       
const int GRN_M = 368;
const int WHT_M = 440;

const int BLK_R = 220;
const int GRN_R = 250; 
const int WHT_R = 345;

const int TRS_L_1 = (BLK_L + GRN_L) / 2;
const int TRS_L_2 = (GRN_L + WHT_L) / 2;
const int TRS_M_1 = (BLK_M + GRN_M) / 2;
const int TRS_M_2 = (GRN_M + WHT_M) / 2;
const int TRS_R_1 = (BLK_R + GRN_R) / 2;
const int TRS_R_2 = (GRN_R + WHT_R) / 2;

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
  const int  fpwr = 60;
  const int  bpwr = -50;
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
  if(rhtval < TRS_R_1 && state!=VEER_RIGHT ){  //right sees black
    setbothSpeeds(fpwr, bpwr);
    state = VEER_RIGHT;
  }
  else if(lftval > TRS_L_2 && rhtval > TRS_R_2 && state!=VEER_STRAIGHT){  //left and right sees white
    setbothSpeeds(fpwr, fpwr);
    state=VEER_STRAIGHT;
  }
  else if(lftval < TRS_L_1 && rhtval >TRS_R_2 && state!=VEER_LEFT){ // left sees black and right sees white
    setbothSpeeds(bpwr,fpwr);
    state=VEER_LEFT;
  }
  
  char buf[10];
  sprintf(buf, "%d %d %d", lftval, midval, rhtval);
  Serial.println(buf);
}
