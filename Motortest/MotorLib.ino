#include <PololuQik.h>
#include <SoftwareSerial.h>

PololuQik2s12v10 qik(8, 9, 10); // Object of our Pololu Qik; hooked up to pins 8,9,10
volatile long encM0 = 0;

void initMotors() //call this in your setup function or else no motors for anyone
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
