//#include <MotorLib.h>

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 150;
const int GRN_L = 192; 
const int WHT_L = 277;

const int BLK_M = 333;       
const int GRN_M = 363;
const int WHT_M = 486;

const int BLK_R = 212; 
const int GRN_R = 248; 
const int WHT_R = 330;

const int TRS_L_1 = (BLK_L + GRN_L) / 2;
const int TRS_L_2 = (GRN_L + WHT_L) / 2;
const int TRS_M_1 = (BLK_M + GRN_M) / 2;
const int TRS_M_2 = (GRN_M + WHT_M) / 2;
const int TRS_R_1 = (BLK_R + GRN_R) / 2;
const int TRS_R_2 = (GRN_R + WHT_R) / 2;

const int fpwr = 40;
const int bpwr = -30;

int LFT, MID, RHT;

/*
              Left  Middle  Right
Black          80    18      68
Black-Green    
Green          127   50      120
Green-White
White          177   87      178
*/

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
  if (getRHT() > TRS_R_1 && getRHT() < TRS_R_2)
  {
    digitalWrite(13, HIGH); //turn 90 right
    specDistTurn(90, 30, true); //setbothSpeeds(50, -30);
    Serial.println("green");
    digitalWrite(13, LOW);
  }
  else if (getLFT() > TRS_L_1 && getLFT() < TRS_R_2)
  {
    digitalWrite(13, HIGH); //turn 90 left
    specDistTurn(90, 30, false); //setbothSpeeds(-30, 50);
    Serial.println("green");
    digitalWrite(13, LOW);
  }
  else if (getMID() < TRS_M_1)
  {
    setbothSpeeds(30, 30);
   
    //go forward
  }
  else //left and right do not see green, middle does not see black
  {
    
    if (getRHT() < TRS_R_1)
    {
      if (getLFT() < TRS_L_1)
      {
        setbothSpeeds(30,30);
      }
      else if (getLFT() > TRS_L_2)
      {
        setbothSpeeds(fpwr,bpwr);
        //right
      }
    }
    else if (getRHT() > TRS_R_2)
    {
      if (getLFT() < TRS_L_1)
      {
        setbothSpeeds(bpwr,fpwr);
        //left
      }
      else if (getLFT() > TRS_L_2)
      {
        setbothSpeeds(30,30);
        //forward
      }
    }
    else
    {
      setbothSpeeds(30,30);
      //forward
    }
  }
  Serial.print(getLFT());
  Serial.print(" ");
  Serial.print(getMID());
  Serial.print(" ");
  Serial.println(getRHT()); 
}
