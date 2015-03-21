const int analogInPinOne = A0;
const int analogInPinTwo = A1;
const int analogInPinThree = A2;

const int BLK_L = 80;
const int BLK_M = 18;
const int BLK_R = 68;
const int GRN_L = 127;
const int GRN_M = 50;
const int GRN_R = 120;
const int WHT_L = 177;
const int WHT_M = 87;
const int WHT_R = 178;

const int TRS_L_1 = (BLK_L + GRN_L) / 2;
const int TRS_L_2 = (GRN_L + WHT_L) / 2;
const int TRS_M_1 = (BLK_M + GRN_M) / 2;
const int TRS_M_2 = (GRN_M + WHT_M) / 2;
const int TRS_R_1 = (BLK_R + GRN_R) / 2;
const int TRS_R_2 = (GRN_R + WHT_R) / 2;

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
}

void loop() 
{
  if (getRHT() > TRS_R_1 && getRHT() < TRS_R_2)
  {
    //turn right
  }
  else if (getLFT() > TRS_L_1 && getLFT() < TRS_R_2)
  {
    //turn left
  }
  else if (getMID() < TRS_M_1)
  {
    //go forward
  }
  else
  {
    if (getRHT() < TRS_R_1)
    {
      if (getLFT() < TRS_L_1)
      {
        //nothing
      }
      else if (getLFT() > TRS_L_2)
      {
        //right
      }
    }
    else if (getRHT() > TRS_R_2)
    {
      if (getLFT() < TRS_L_1)
      {
        //left
      }
      else if (getLFT() > TRS_L_2)
      {
        //forward
      }
    }
    else
    {
      //forward
    }
  }
}
