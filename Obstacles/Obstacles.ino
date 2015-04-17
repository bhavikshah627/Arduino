#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int FSHARP = A3;

float getSideDist(int x)
{ //88 - 441
  if (x < 80 || x > 450)
    return -1;
  else
    return  3888.2 * pow(x, -1.237);
}

float getFrontDist(int x)
{
  if (x < 80 || x > 560)
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
  specDistTurn(90, 50, true);
  if (rightDist == -1 || leftDist == -1 || rightDist == leftDist)
    return true;
  else if (rightDist > leftDist )
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
  Serial.println(idist);
  Serial.println(deg);
  return (idist * abs(tan(deg * (3.14 / 180))));
}

void obstAvoidP1(float cm)
{
  float radius = cm;
  float bufferZone = 5; //space in between robot and obstacle -- change as needed
  float tpwr = 70; //taget power level (0-127)
  float Width = 14;
  float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"u
  float outDia = inDia + Width;
  float pwrRatio = inDia / outDia;

  boolean side = true; //SHOULD EQUAL CHECKSIDE, CHANGED FOR TESTING
  //does initial turn
  if (side)
    specDistTurn(65, 50, true);
  if (!side)
    specDistTurn(65, 50, false);
  if (side)
    setbothSpeeds(tpwr * pwrRatio, tpwr);
  else if (!side)
    setbothSpeeds(tpwr, tpwr * pwrRatio  );

  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);  // turn off LED

  while (clear > 1600)
  {
    tcs.setInterrupt(false);      // turn on LED
    delay(60);  // takes 50ms to read
    tcs.getRawData(&red, &green, &blue, &clear);
    tcs.setInterrupt(true);  // turn off LED
    Serial.println(clear);
  }
  Serial.println(clear);

  stopMotors();
}

void setup()
{
  Serial.begin(9600);
  initMotors();
  tcs.begin();
  Serial.println(calcRadius(true));

}

void loop()
{

}
