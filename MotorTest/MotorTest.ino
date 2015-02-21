
extern int x;

void setup()
{
	Serial.begin(9600);
	//qik.init();
        initMotors();
       // encM0=0;
        x=10;
}
void loop()
{
	//qik.setM0speed(127);
	Serial.print(getRotations());
}
