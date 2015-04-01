void setup() 
{
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() 
{
  char buf[15];
  sprintf(buf,"%d %d",analogRead(A0),analogRead(A1));
  Serial.println(buf);
//  Serial.print(analogRead(A0) );
//  Serial.print("  ");
//  Serial.println(analogRead(A1));
}
