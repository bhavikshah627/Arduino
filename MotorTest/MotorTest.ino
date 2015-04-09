
void setup()
{
  Serial.begin(9600);
  initMotors();
}
void loop()
{
    specDistTurn(90, 30, true);
    while(true)
    {
    }
}
