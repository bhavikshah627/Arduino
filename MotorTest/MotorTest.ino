
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{
    setbothSpeeds(100, 100);
}
