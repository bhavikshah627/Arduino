//#include <MotorLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int B_L = 1110;
//const int G_L = 2014;
const int W_L = 3485;

const int B_M = 946;
const int W_M = 3558;

const int B_R = 1180;
//const int G_R = 2165;
const int W_R = 3805;;

const int T_L = (B_L + W_L) / 2;
//const int T_L_2 = (G_L + W_L) / 2;
const int T_M = (B_M + W_M) / 2;
const int T_R = (B_R + W_R) / 2;
//const int T_R_2 = (G_R + W_R) / 2;

//use of states to prevent overuse of serial
const int VEER_NONE = 0;
const int VEER_LEFT  = 1;
const int VEER_RIGHT  = 2;
const int VEER_STRAIGHT = 3;
const int FSHARP = A3;
int muxports[] = {1, 3};
//left, middle, right;
const int numports = sizeof(muxports) / sizeof(int);
uint16_t clear[2], red[2], blue[2], green[2], mclear, mred, mblue, mgreen;
Adafruit_TCS34725 tcsleft = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void getMidC()
{
  if (mux(0)) Serial.println("Yo");
  tcsleft.getRawDataEx(&mred, &mgreen, &mblue, &mclear);
}

void getSideC()
{
  int index = 0;
  for (index = 0; index < numports; index++)
  {
    if (mux(muxports[index])) Serial.println("Yo");
    tcsleft.getRawDataEx(&red[index], &green[index], &blue[index], &clear[index]);
  }
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
    setbothSpeeds(35, -35);
  else
    setbothSpeeds(-35, 35);
  float x = getFrontDist(analogRead(FSHARP));
  while (x < 30 && x != -1) {
    x = getFrontDist(analogRead(FSHARP));
  }
  stopMotors();
  float deg = getEncoderVal() / encperDEG;
  Serial.println(idist);
  Serial.println(deg);
  return (idist * abs(tan(deg * (3.14 / 180))));
}

void obstAvoidP1()
{
  boolean side = true;
  float bufferZone = 5; //space in between robot and obstacle -- change as needed
  float tpwr = 70; //taget power level (0-127)
  float Width = 14;

  float radius = calcRadius(side);
  stopMotors();
  delay(1000);
  float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"u
  float outDia = inDia + Width;
  float pwrRatio = inDia / outDia;

  if (side)
    specDistTurn(40, 50, true);
  else if (!side)
    specDistTurn(40, 50, false);
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


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  TWBR = 12;
  for (int i = 0; i < numports; i++)
  {
    if (mux(muxports[i])) Serial.println("ERROR MUX");
    Serial.print("Init sensor ");
    Serial.println(muxports[i]);
    if (tcsleft.begin())
    {
      Serial.println("Found sensor");
      tcsleft.setInterrupt(false);
    }
    else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  }

  if (mux(0)) Serial.println("ERROR MUX");
  Serial.print("Init sensor ");
  Serial.println(0);
  if (tcsleft.begin())
  {
    Serial.println("Found sensor");
    tcsleft.setInterrupt(false);
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  delay(60);
  initMotors();
  delay(1000);
}

void loop()
{
  static int state = VEER_NONE;
  const int  fpwr = 30;
  const int  bpwr = -20;
  getSideC();
  getMidC();
  char buf[40];
  sprintf(buf, "%d  %d %d %d %d %d", clear[0], blue[0], green[0], clear[1], blue[1], green[1]);
  Serial.println(buf);
  int dist = getFrontDist(analogRead(FSHARP));
  if(dist<12 && dist != -1)
  {
    obstAvoidP1();
  }
  else if (blue[0] < 400 && green[0] > 600)
  { //left green --only sometimes work
    stopMotors();
    delay(500);
    specDist(7, 30);
    while (mclear < T_M)
    {
      getMidC();
      setbothSpeeds(-30, 30);
    }
    while (mclear > T_M)
    {
      setbothSpeeds(-30, 30);
      getMidC();
    }
  }
  else if (blue[1] < 400 && green[1] > 600)
  { //right green --only sometimes work
    stopMotors();
    delay(500);
    specDist(7, 30);
    while (mclear < T_M)
    {
      getMidC();
      setbothSpeeds(30, -30);
    }
    while (mclear > T_M)
    {
      setbothSpeeds(30, -30);
      getMidC();
    }
  }
  else if (clear[1] < T_R)
  { //right sees black
    if (mclear < T_M)
    { //middle also sees black
      specDist(1, fpwr);
      //specDistTurn(5, fpwr, false);
    }
    else if (state != VEER_RIGHT)
    {
      Serial.println("RIGHT");
      setbothSpeeds(fpwr, bpwr);
      state = VEER_RIGHT;
    }
  }
  else if (clear[0] < T_L  && clear[1] > T_R && state != VEER_LEFT)
  { // left sees black and right sees white
    setbothSpeeds(bpwr, fpwr);
    state = VEER_LEFT;
  }
  else if (clear[0] > T_L  && clear[1] > T_R)
  { // left sees white and right sees white
    setbothSpeeds(fpwr, fpwr);
    state = VEER_STRAIGHT;
  }
}
