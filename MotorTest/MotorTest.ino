
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{
    setbothSpeeds(50, -50);
    delay(3000);
    setbothSpeeds(-50, 50);
    delay(3000);
}
