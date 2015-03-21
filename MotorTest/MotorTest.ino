
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{   
  setbothSpeeds(0, 127);
  
  Serial.println(getRotations());
}
