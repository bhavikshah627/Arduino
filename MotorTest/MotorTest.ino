
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{
    setbothSpeeds(30, 30);
}
