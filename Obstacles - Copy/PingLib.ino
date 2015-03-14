//basic PingLibrary

int getPingDist(int pingPin)
{
	// establish variables for duration of the ping, 
	// and the distance result in inches and centimeters:
	long duration, inches, cm;

	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);

	pinMode(pingPin, INPUT);
	duration = pulseIn(pingPin, HIGH);

	cm = microsecondsToCentimeters(duration);

	return cm;
}

void setDist(int tDist, int pingPin) //this requires motorLib to function
{
  //adjusts the robot until distance from ping to object is specified dist
  int curDist = getPintDist(pingPin)
  if(curDist < tDist)
  {
    while(curDist < tDist)
    {
      int curDist = getPintDist(pingPin)
      setbothSpeeds(-20, -20);
    }
  }
  if(curDist > tDist)
  {
    while(curDist > tDist)
    {
      int curDist = getPintDist(pingPin)
      setbothSpeeds(20, 20);
    }
  }  
}
long microsecondsToCentimeters(long microseconds)
{
	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
	// The ping travels out and back, so to find the distance of the
	// object we take half of the distance travelled.
	return microseconds / 29 / 2;
}
