

const int FSHARP = A3;

float getSideDist(int sharpPin)
{
  return  3888.2 * pow(analogRead(sharpPin), -1.237);
}

float getFrontDist(int x)
{
    if(x < 80 || x > 560)
      return -1;
    else
       return 9780.9* pow(x,-1.09);
}

boolean checkSide()
{ //returns true if rightside is greater, left is false
  specDistTurn(90, 70, true);
  int rightDist = getFrontDist(analogRead(FSHARP));
  specDistTurn(180, 70, false);
  int leftDist = getFrontDist(analogRead(FSHARP));
  //goes back to original side 
  specDistTurn(90, 70, true);
  if(rightDist>leftDist)
    return true;
  if(leftDist>rightDist)
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
 float pwrRatio = inDia/outDia;
 
 boolean side = checkSide();
 //does initial turn
 if(side)
   specDistTurn(60, 70, true);
 if(!side)
    specDistTurn(60, 70, false);
    
   //goes foward to avoid seeing the line it's on
  specDist(5, 40);
 
 //while(lightvals[0] > blackval)
 //{ //getlightvals();
 if(side)
   setbothSpeeds(tpwr * pwrRatio, tpwr);
  else if(!side)
    setbothSpeeds(tpwr, tpwr *pwrRatio  );
 //}
}

void setup()
{
  Serial.begin(9600);
  initMotors();
}

void loop()
{
  int x = analogRead(FSHARP);
  setbothSpeeds(50, 50);
  float dist = getFrontDist(x);
  if(dist < 11 && dist != -1)
  {
    stopMotors();
    while(true)
    {
      
    }
  }
}
