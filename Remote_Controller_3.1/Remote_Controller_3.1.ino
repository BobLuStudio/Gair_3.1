
bool mode1 = 0, mode2 = 0, mode3 = 0, mode4 = 0;
float sensorx = 0, sensory = 0; //to correct the data.
uint8_t tpower;
float statex, statey, statez;
void setup()
{
  Serial.begin(57600);
  Serial2.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(A0, INPUT);
  reset();
  while (potentiometer() > 1 || digitalRead(11) == HIGH || digitalRead(12) == HIGH || digitalRead(19) == HIGH || digitalRead(18) == HIGH)
  {
    digitalWrite(10, HIGH);
  }
  digitalWrite(10, LOW);
  pinMode(A0, INPUT); //the port of power
  delay(300);
  digitalWrite(10, HIGH);
  delay(300);
  digitalWrite(10, LOW);
}
void loop()
{
	updateModes();
	sendCommand();
  /*
    if(Serial.available())
    {
    if(Serial2.read()=='h')
    {
    digitalWrite(10,HIGH);
    }
    if(Serial2.read()=='l')
    {
    digitalWrite(10,LOW);
    }
    }
  */
}

void updateModes()
{

	tpower = potentiometer(); //get the total power from potentiometer
	state();
	mode4 = digitalRead(19);
	mode3 = digitalRead(18);
	mode2 = digitalRead(12);
	mode1 = digitalRead(11);
}

void sendCommand()
{
  uint8_t mode=0;
	if (mode4)mode |= 1 << 3; //land
	else
		mode &= ~(1 << 3);
    
    if (mode3)mode |= 1<<2; //take off
	else
		mode &= ~(1 <<2);
   
    if (mode2)mode |= 1<<1; //keep
	else
		mode &= ~(1 << 1);
    
    if (mode1)mode |= 1; //stop
	else
		mode &= ~(1);

  char commandString[8];
  commandString[0]=(char)mode;
  floatSplitter(&commandString[1], statex);
  floatSplitter(&commandString[3], statey);
  floatSplitter(&commandString[5], statez);
  commandString[7]=(char)tpower;

  
  printOut('$');//0
  uint8_t checkByte=0;
  for(uint8_t i=0;i<8;i++)//8 bytes
  {
    checkByte^=commandString[i];
    printOut(commandString[i]);
  }
  printOut((char)checkByte);//9
  printOut('&');//10

}

void printOut(char rsc)
{
  Serial2.print(rsc);//U can add some encryption algorithm here
}

void floatSplitter(char elements[2], float rsc)
{
  int16_t rscInt = rsc * 100;
  elements[0] = (char)rscInt;
  elements[1] = (char)*((uint8_t*)(&rscInt)+1);
}

