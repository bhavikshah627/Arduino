
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{   
  setbothSpeeds(0, 50);
  
  Serial.println(getRotations());
}
