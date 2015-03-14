\\#include <PololuQik.h>
#include <SoftwareSerial.h>


//all turning is under current assumption that \
private PololuQik2s12v10 qik(8, 9, 10); // Object of our Pololu Qik; hooked up to pins 8,9,10
private volatile long encM0 = 0;
float tireD = 3.53; //cm of tire diameter
float WB = 12.01; //cm of the WB + half the wheels
float pi = 3.14;
private float encperCM = 564/(tireD*pi);
private float encperDEG = ((WB*pi)*encperCM)/360;
void initMotors() //call this in your setup function or else no motors for you :(
{

  	qik.init();
	attachInterrupt(0, EncoderA, RISING);
        encM0 = 0;
}

void EncoderA() //that danged encoder -- interupting everything
{
  encM0++;
}

void stopMotors()
{
  qik.setM0Speed(0);
  qik.setM1Speed(0);
}

void resetEncoder()
{
  encM0 = 0;
}

void setM0speed(int speed)
{
  qik.setM0Speed(speed);
}

void setM1speed(int speed)
{
  qik.setM1Speed(speed);
}

void setbothSpeeds(int s1, int s2)
{
  qik.setM0Speed(s1);
  qik.setM1Speed(s2);
}
float getRotations()
{
  return (encM0/564);
}

long getEncoderVal()
{
  return encM0;
}

void specDist(int tCM, int tSpeed) //if tSpeed is negative it goes back
{
  int target = tCM * encperCM; 
  stopMotors();
  resetEncoder();
  while(encM0 < target)
  {
    setbothSpeeds(tSpeed, tSpeed);
  }  
}

void specDistTurn(int tDeg, int tSpeed, boolean turn) // true = turnright, false = turnleft
{
 int target = tDeg * encperDEG;
 stopMotors();
 resetEncoder();
 while(encM0<target)
 {
   if(turn)
     setbothSpeeds(tSpeed, -tSpeed);
   else
     setbothSpeeds(-tSpeed, tSpeed);
}
