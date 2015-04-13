int readvalue;
const int analogPinOne = A0;
const int analogPinTwo = A2;
const int analogPinThree = A1;

const int cl_0 = 65;  //black
const int cl_1 = 75;  //dark green
const int cl_2 = 130; //normal green
const int cl_3 = 150; //light green
const int cl_4 = 200; //white

int trs_1 = (cl_0 + cl_1)/2;
int trs_2 = (cl_1 + cl_2)/2;
int trs_3 = (cl_2 + cl_3)/2;
int trs_4 = (cl_3 + cl_4)/2;

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  char buf[10];
  sprintf(buf, "%d, %d, %d", analogRead(analogPinOne), analogRead(analogPinTwo), analogRead(analogPinThree));
  Serial.println(buf);
  delay(100);
  
  /*if (readvalue < trs_1)
    Serial.print(" Black");
  else if (readvalue >= trs_1 && readvalue < trs_2)
    Serial.print(" Dark Green");
  else if (readvalue >= trs_2 && readvalue < trs_3)
    Serial.print(" Normal Green");
  else if (readvalue >= trs_3 && readvalue < trs_4)
    Serial.print(" Light Green");
  else if (readvalue >= trs_4)
    Serial.print(" White");
  Serial.print("\n");*/
}
