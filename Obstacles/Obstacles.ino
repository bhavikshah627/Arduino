
const int FSHARP = A3;

float getSideDist(int x)
{ //88 - 441
if(x<80 || x>450)
  return -1;
 else
    return  3888.2 * pow(x, -1.237);
}

float getFrontDist(int x)
{
  if (x < 80 || x > 560)
    return -1;
  else
    return 9780.9 * pow(x, -1.09);
}



boolean checkSide()
{ //returns true if rightside is greater, left is false
  specDistTurn(90, 50, true);
  delay(500);
  
  int rightDist = getFrontDist(analogRead(FSHARP));
  specDistTurn(180, 50, false);
  delay(500);
  
  int leftDist = getFrontDist(analogRead(FSHARP));
  //goes back to original side
  specDistTurn(90, 50, true);
  if(rightDist == -1 || leftDist == -1 || rightDist == leftDist)
    return true;
  else if (rightDist > leftDist )
    return true;
  else if (leftDist > rightDist)
    return false;
}

void obstAvoidP1(int cm)
{
  //Uses an asssumed value to go around obstacle
  //Assuming radius is 10 cm
  float radius = 10;
  float bufferZone = 5; //space in between robot and obstacle -- change as needed
  float tpwr = 70; //taget power level (0-127)
  float Width = 14;
  float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"
  float outDia = inDia + Width;
  float pwrRatio = inDia / outDia;

  boolean side = checkSide();
  //does initial turn
  if (side)
    specDistTurn(60, 50, true);
  if (!side)
    specDistTurn(60, 50, false);

  //goes foward to avoid seeing the line it's on
  specDist(5, 40);
  //while loop
  if (side)
    setbothSpeeds(tpwr * pwrRatio, tpwr);
  else if (!side)
    setbothSpeeds(tpwr, tpwr * pwrRatio  );
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int x = analogRead(A0);
  Serial.print(x);
  Serial.print("\t");
  Serial.println(getSideDist(x));
  delay(1000);
  /*int x = analogRead(FSHARP);
  setbothSpeeds(50, 50);
  float dist = getFrontDist(x);
  Serial.println(dist);
  if (dist < 11 && dist != -1)
  {
    stopMotors();
    while (true) { }
  }*/

}
