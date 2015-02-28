
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{   
  setM0speed(50);
  
  Serial.println(getRotations());
}
