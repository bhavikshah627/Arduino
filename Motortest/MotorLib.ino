#include <PololuQik.h>
#include <SoftwareSerial.h>

private PololuQik2s12v10 qik(8, 9, 10); // Object of our Pololu Qik; hooked up to pins 8,9,10
private volatile long encM0 = 0;
private float tireD = 3.53; //cm of tire diameter
private float WB = 12.01; //cm of the WB + half the wheels
private float pi = 3.14;
private float encperCM = 564/(tireD*pi);
private float encperDEG = (WB*pi)
void initMotors() //call this in your setup function or else no motors 
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

void specDist(int tCM)
{
  
}
