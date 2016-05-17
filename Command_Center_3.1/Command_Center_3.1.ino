
#define Xaxis 0
#define Yaxis 1
#define Zaxis 2


float rstate[3], ustate[3];
uint8_t rtpower;  // total power in remote's command
int mode;
bool modes[4];
uint16_t count[3];
int8_t fanPower;


struct ToUAV
{
  public:
    void (*printOut)(char rsc);
    void sendRemoteCommand()
    {
      char commandString[8];
      commandString[0] = (char)mode;
      floatSplitter(&commandString[1], rstate[Xaxis]);
      floatSplitter(&commandString[3], rstate[Yaxis]);
      floatSplitter(&commandString[5], rstate[Zaxis]);
      commandString[7] = (char)rtpower;


      printOut('$');//0
      uint8_t checkByte = 0;
      for (uint8_t i = 0; i < 8; i++) //8 bytes
      {
        checkByte ^= commandString[i];
        printOut(commandString[i]);
      }
      printOut((char)checkByte);//9
      printOut('&');//10
    }
  protected:
    void floatSplitter(char elements[2], float rsc)
    {
      int16_t rscInt = rsc * 100;
      elements[0] = (char)rscInt;
      elements[1] = (char) * ((uint8_t*)(&rscInt) + 1);
    }
} toUAV;

struct ToUser : ToUAV
{
  public:
    void sendUAVState()
    {
      char commandString[8];
      commandString[0] = (char)mode;
      floatSplitter(&commandString[1], ustate[Xaxis]);
      floatSplitter(&commandString[3], ustate[Yaxis]);
      floatSplitter(&commandString[5], ustate[Zaxis]);
      commandString[7] = (char)rtpower;


      printOut('$');//0
      uint8_t checkByte = 0;
      for (uint8_t i = 0; i < 8; i++) //8 bytes
      {
        checkByte ^= commandString[i];
        printOut(commandString[i]);
      }
      printOut((char)checkByte);//9
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
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  analogWrite(9, 0);
  count[3] = 50000;
}

void loop()
{
  if (fromremote())
  {
    toUAV.sendRemoteCommand();
    toUser.sendRemoteCommand();
    digitalWrite(6, HIGH);
    count[0] = 0;
  }
  else
  {
    count[0]++;
    if (count[0] > 10000)
    {
      digitalWrite(6, LOW);
      rstate[0] = 0;
      rstate[1] = 0;
      rstate[2] = 0;
      toUAV.sendRemoteCommand();
      toUser.sendRemoteCommand();
    }
  }


  if (Serial2.available() > 0)
  {
    if (fromUAV())
    {
      toUser.sendUAVState();
      digitalWrite(7, HIGH);
    } else
      digitalWrite(7, LOW);
  }

  count[2]++;
  if (count[2] > 50000)
  {
    count[2] = 0;
    Serial.println(analogRead(A1));
    fanPower = ((analogRead(A1) + analogRead(A0)) * 3) >> 2;
    if (fanPower > 255)fanPower = 255;
    if (fanPower < 0)fanPower = 0;
    analogWrite(9, fanPower);
  }

}


