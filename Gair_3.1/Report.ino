
#define reportPosition 10000

void printOut(char rsc)
{
  Serial3.print(rsc);//U can add some encryption algorithm here
}

void floatSplitter(char elements[2], float rsc)
{
	int16_t rscInt = rsc * 100;
	elements[0] = (char)rscInt;
	elements[1] = (char)*((uint8_t*)(&rscInt) + 1);
}

void transmiteFloat(float rsc)
{
  char elements[2];
  floatSplitter(elements, rsc);
  printOut(elements[0]);
  printOut(elements[1]);
}

void ReportState()
{

  uint8_t numofcommand = (uint8_t)(rstate[Xaxis] + rstate[Yaxis] + rstate[Zaxis]);

  printOut('$');//0
  transmiteFloat(rstate[Xaxis]);//1,2
  transmiteFloat(rstate[Yaxis]);//3,4
  transmiteFloat(rstate[Zaxis]);//5,6
  printOut((char)numofcommand);//8
  printOut('&');//9
}
void reportGPS()
{
	GPS.GetInform();

  
  uint8_t numofcommand = (uint8_t)(GPS.speed_Knots+GPS.longitude+GPS.attitude);

  printOut('$');//0
  printOut((char)GPS.speed_Knots);//1
  printOut((char)GPS.longitude);
  printOut((char)numofcommand);
  printOut('&');
}
void Report(uint16_t index)
{
  switch (index)
  {
    case reportPosition: reportGPS();break;
    default: ReportState(); break;
  }
}


