//Last updated 3/7/15
//Uses an asssumed value to go around obstacle

void setup() 
{
  Serial.begin(9600);
  initMotors();
}

void loop() 
{
  //Assuming radius is 10 cm
 float radius = 10; 
 float bufferZone = 5; //space in between robot and obstacle -- change as needed
 float tpwr = 70; //taget power level (0-127)
 float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"
 float outDia = inDia = WB;
 
}
