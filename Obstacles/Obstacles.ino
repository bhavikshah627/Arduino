#include <Wire.h>
#include "Adafruit_TCS34725.h"


const int FSHARP = A3;

int muxports[] = {1, 3};
//left, middle, right;
const int numports = sizeof(muxports) / sizeof(int);
uint16_t clear[2], red[2], blue[2], green[2], mclear, mred, mblue, mgreen;
Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


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

void getMidC()
{

  if (mux(0)) Serial.println("Yo");
  tcsleft.getRawDataEx(&mred, &mgreen, &mblue, &mclear);

}


float getSideDist(int x)
{ //88 - 441
  if (x < 80 || x > 450)
    return -1;
  else
    return  3888.2 * pow(x, -1.237);
}

float getFrontDist(int x)
{
  if (x < 80 || x > 580)
    return -1;
  else
    return 9780.9 * pow(x, -1.09);
}

boolean checkSide()
{
  //returns true if rightside is greater, left is false
  specDistTurn(90, 60, true);
  delay(500);

  int rightDist = getFrontDist(analogRead(FSHARP));
  specDistTurn(180, 50, false);
  delay(500);

  int leftDist = getFrontDist(analogRead(FSHARP));
  //goes back to original side
  Serial.println(rightDist);
  Serial.println(leftDist);
  specDistTurn(90, 50, true);
  if (rightDist == -1  || rightDist == leftDist)
    return true;
  else if (leftDist == -1)
    return false;
  else if (rightDist > leftDist)
    return true;
  else if (leftDist > rightDist)
    return false;
}

extern float encperDEG;

float calcRadius(boolean side)
{
  resetEncoder();
  float idist = getFrontDist(analogRead(FSHARP));
  if (side)
    setbothSpeeds(25, -25);
  else
    setbothSpeeds(-25, 25);
  while (getFrontDist(analogRead(FSHARP)) < 30) {}
  stopMotors();
  float deg = getEncoderVal() / encperDEG;
 // Serial.println(idist);
  //Serial.println(deg);
  return (idist * abs(tan(deg * (3.14 / 180))));
}

void obstAvoidP1()
{
  boolean side = checkSide();
  float bufferZone = 5; //space in between robot and obstacle -- change as needed
  float tpwr = 70; //taget power level (0-127)
  float Width = 14;



  float radius = calcRadius(checkSide());

  float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"u
  float outDia = inDia + Width;
  float pwrRatio = inDia / outDia;




  //does initial turn

  if (side)
    specDistTurn(20, 50, true);
  if (!side)
    specDistTurn(20, 50, false);
  if (side)
    setbothSpeeds(tpwr * pwrRatio, tpwr);
  else if (!side)
    setbothSpeeds(tpwr, tpwr * pwrRatio  );

  getMidC();

  while (mclear > 1600)
  {
    getMidC();
  }

  stopMotors();
}

void setup()
{
  Serial.begin(9600);
  initMotors();
  Wire.begin();
  TWBR = 12;
  if (mux(0))Serial.println("R.I.P. Mux");
  tcsleft.begin();
  delay(60);
  

}

void loop()
{
  // Serial.println(getFrontDist(analogRead(FSHARP)));
}
