//#include <MotorLib.h>

const int analogInPinOne = A0;
const int analogInPinTwo = A2;
const int analogInPinThree = A1;

const int BLK_L = 145;
const int WHT_L = 265;
const int BLK_R = 115;
const int WHT_R = 185;

const int TRS_L = (BLK_L + WHT_L) / 2;
const int TRS_R = (BLK_R + WHT_R) / 2;

int LFT, MID, RHT;

/*
              Left  Middle  Right
Black          80    18      68
Black-Green
Green          127   50      120
Green-White
White          177   87      178
*/

int getLFT() {
  return analogRead(analogInPinOne);
}
int getMID() {
  return analogRead(analogInPinTwo);
}
int getRHT() {
  return analogRead(analogInPinThree);
}

void setup()
{
  Serial.begin(9600);
  initMotors();
}

void loop()
{
  if (getRHT() < TRS_R)
  {
    if (getLFT() < TRS_L)
    {
      setbothSpeeds(30, 30);
    }
    else if (getLFT() > TRS_L)
    {
      setbothSpeeds(25, 0);
      //right
    }
  }
  else if (getRHT() > TRS_R)
  {
    if (getLFT() < TRS_L)
    {
      setbothSpeeds(0, 25);
      //left
    }
    else if (getLFT() > TRS_L)
    {
      setbothSpeeds(30, 30);
      //forward
    }
  }
  else
  {
    setbothSpeeds(30, 30);
    //forward
  }

  Serial.print(getLFT());
  Serial.print(" ");
  Serial.print(getMID());
  Serial.print(" ");
  Serial.println(getRHT());
}
