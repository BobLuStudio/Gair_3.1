int mtime = 0, prevmtime, dmtime;
#include <Servo.h>

#define d1t 22   //set distant sensor port
#define d1e 23
#define d2t 52
#define d2e 53
#define d3t 2
#define d3e 3
#define d4t 7
#define d4e 8

#define fmpin  0  //set motor port
#define bmpin  1
#define lmpin  2
#define rmpin  3

#define PIDx 0
#define PIDy 1
#define PIDz 2

#define Xaxis 0
#define Yaxis 1
#define Zaxis 2

#define landMode 3
#define takeOffMode 2
#define keepMode 1
#define stopMode 0

int motorPins[4];
int takeoffpower = 0, landpower = 0;
float avd = 0; //to store average distance
float Kpd = 0.2, Kid = 0.07, Kdd = 0.07;//vertical PID
float preverrodistance = 0, totalPIDid = 0;
unsigned int dtimed = 0, btimed = 0;//dtimed:difference of time;btimed:before time piont(for distance PID)

float ustate[3] = {0, 0, 0}; //state of uav{x,y,z}
float rstate[3] = {0, 0, 0}; //remote data{x,y,z}
uint16_t reportIndex = 0;
uint8_t mode;
float fpower, rpower, lpower, bpower, tpower = 0; //power of every moto to apply
int dotlocation;//to judge where the dot is in order to analyze date form state sensor
int sensorx = 0, sensory = 0; //to correct state sensor data
bool modes[4];//special mode
unsigned int location;//to judge where the dot is in order to analyze date form remote
unsigned long time = 0, btime, dtime; //to store the time scince this program start,last time point,diference time between two point
//PID
float PIDex[2] = {0, 0}; //erro of state between remote and nav, previous diference of state between remote and nav,
float PIDey[2] = {0, 0};
float PIDez[2] = {0, 0};

float PIDeAccx[2] = {0, 0}; //erro of state between remote and nav, previous diference of state between remote and nav,
float PIDeAccy[2] = {0, 0};
float PIDeAccz[2] = {0, 0};

float OUTPIDx[3] = { -1.84, 0, 0.45}; //p,i,d{-1.84, 0, 0.45};
float OUTPIDy[3] = { -1.8, 0, 0.34}; //{-1.8, 0, 0.34};
float OUTPIDz[3] = {0, 0, 0};
float KPIDx[3] = { -0.5, -0.2, -1.2}; //{-0.5, -0.2, -1.2};
float KPIDy[3] = { -0.5, -0.2, -1.2}; //{-0.5, -0.2, -1.2};
float KPIDz[3] = {0, 0, 0};
char oneline[34];//to store the command(base data)
float*OUTPID[3] = {OUTPIDx, OUTPIDy, OUTPIDz}; //x,y,z
float*INNERPID[3] = {KPIDx, KPIDy, KPIDz};
float*PIDe[3] = {PIDex, PIDey, PIDez};
float*PIDeAcc[3] = {PIDeAccx, PIDeAccy, PIDeAccz};
float Acceleration[3];
float PIDi[3] = {0, 0, 0};
float OUTPIDi[3] = {0, 0, 0};
float PIDout[3] = {0, 0, 0}; //out put of pid
float prevPID[3] = {0, 0, 0};
//float OUTPIDiMAX[3] = {3, 3, 3};
float PIDiMAX[3] = {10, 10, 10};
//int PIDMAX[3]={20,20,20};
#define kPIDMAX 0.5
int PIDDifferenceMAX[3] = {20, 20, 20};
//PID
Servo powerServo[4];

void setup()
{

  motorPins[0] = 5; //f
  motorPins[1] = 11; //b
  motorPins[2] = 10; //l
  motorPins[3] = 6; //r
  for (int i = 0; i < 4; i++)
  {
    powerServo[i].attach(motorPins[i]);
  }
  fpower = 100;
  bpower = 100;
  lpower = 100;
  rpower = 100;
  applypowers();
  delay(3000);
  //rest the serial port
  Serial3.begin(57600);  //remote controller
  Serial2.begin(57600);  //
  Serial1.begin(57600);  //motion sensor

  state();  //get the first state data

  sensorx = 0 - ustate[PIDx];
  sensory = 0 - ustate[PIDy];

  int i = 100;
  while (i >= 0)  //reset the eletronic speed controller
  {
    fpower = i;
    bpower = i;
    lpower = i;
    rpower = i;
    applypowers();
    i--;
  }
  delay(2000);

  while (Serial3.available() != 0) //rest the serial port
  {
    Serial3.read();
  }
  pinMode(13, OUTPUT);
  state();
  while ((ustate[PIDx] != 0 || ustate[PIDy] != 0) && i < 50)
  {
    i++;
    sensorx = 0;
    sensorx = 0;
    state();  //get the first state data
    sensorx = 0 - ustate[PIDx];
    sensory = 0 - ustate[PIDy];
  }

}
//****************************************************************************//
uint16_t count = 0;

void loop()
{
  if (fromremote()) //get expect state data(state of remote)
    count = 0;
  else
    count++;

  while (modes[stopMode])
  {
    fpower = 0;
    bpower = 0;
    lpower = 0;
    rpower = 0;
    applypowers();
    fromremote();
  }
  while (modes[landMode] || count > 50000)
  {
    landrobot();
    fromremote();
  }
  while (modes[keepMode])
  {
    keeprobot();
    fromremote();
  }
  realizestate();
}


