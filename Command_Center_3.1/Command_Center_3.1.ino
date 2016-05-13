
#define Xaxis 0
#define Yaxis 1
#define Zaxis 2


float rstate[3], ustate[3];
uint8_t rtpower;  // total power in remote's command
int mode;
bool modes[4];

struct ToUAV
{
  public:
    void (*printOut)(char rsc);
    void sendRemoteCommand()
    {
      uint8_t numofcommand = (uint8_t)(mode + rstate[Xaxis] + rstate[Yaxis] + rstate[Zaxis] + rtpower);

      printOut('$');//0
      printOut((char)mode);//1
      transmiteFloat(rstate[Xaxis]);//2,3
      transmiteFloat(rstate[Yaxis]);//4,5
      transmiteFloat(rstate[Zaxis]);//6,7
      printOut((char)(rtpower));//8
      printOut((char)numofcommand);//9
      printOut('&');//10

    }
  protected:
    void transmiteFloat(float rsc)
    {
      char elements[2];
      floatSplitter(elements, rsc);
      printOut(elements[0]);
      printOut(elements[1]);
    }

    void floatSplitter(char elements[2], float rsc)
    {
      int16_t rscInt = rsc * 100;
      elements[0] = (char)((rscInt << 8) >> 8);
      elements[1] = (char)(rscInt >> 8);
    }
} toUAV;

struct ToUser : ToUAV
{
  public:
    void sendUAVState()
    {
      uint8_t numofcommand = (uint8_t)(mode + rstate[Xaxis] + rstate[Yaxis] + rstate[Zaxis] + 101);

      printOut('$');//0
      printOut((char)mode);//1
      transmiteFloat(ustate[Xaxis]);//2,3
      transmiteFloat(ustate[Yaxis]);//4,5
      transmiteFloat(ustate[Zaxis]);//6,7
      printOut((char)(101));//8
      printOut((char)numofcommand);//9
      printOut('&');//10

    }
} toUser;

void setup()
{
  Serial.begin(57600);
  Serial2.begin(57600);  //nav
  Serial3.begin(9600);  //remote
  pinMode(2, OUTPUT);

  toUser.printOut = &UserPrintOut;
  toUAV.printOut = &UAVPrintOut;
}

void loop()
{
  if (Serial3.available() > 0)
  {
    if (fromremote())
    {
      toUAV.sendRemoteCommand();
      toUser.sendRemoteCommand();
      /*Serial.print("$");
      Serial.print('x');
      Serial.print(rstate[Xaxis]);
      Serial.print('y');
      Serial.print(rstate[Yaxis]);
      Serial.print('z');
      Serial.print(rstate[Zaxis]);
      Serial.print('t');
      Serial.println(rtpower);*/
    }
  }

  if (Serial2.available() > 0)
  {
    if (fromUAV())
    {
      toUser.sendUAVState();
    }
  }
}


