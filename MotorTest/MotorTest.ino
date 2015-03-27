
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{   
  
  Serial.println(getRotations());
}
