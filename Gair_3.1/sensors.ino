
unsigned int correctstate[3] = {0, 0, 0};
void state()
{
  dotlocation = 0;
  float estatex, estatey, estatez;
  if (Serial1.available() > 0)
  {
    getoneline();
    estatez = Stoint(dotlocation); //recive state data in "x,y,z",and storge in estate
    estatex = Stoint(dotlocation + 4) + sensorx;
    estatey = Stoint(dotlocation + 4) + sensory;
    //storge data in state, if the data is correct,
    if ((estatez < 180) && (estatez > (-180)) && ((abs(estatez - ustate[PIDz]) < 20) || correctstate[PIDz] > 50))
    {
      ustate[PIDz] = estatez;
      if (correctstate[PIDz] != 0)
      {
        correctstate[PIDz] = 0;
      }
    }
    else
    {
      if (abs(estatez - ustate[PIDz]) > 20)
      {
        correctstate[PIDz]++;
      }
    }
    if ((estatex < 180) && (estatex > (-180)) && ((abs(estatex - ustate[PIDx]) < 20 || correctstate[PIDx] > 50)))
    {
      ustate[PIDx] = estatex;
      if (correctstate[PIDx] != 0)
      {
        correctstate[PIDx] = 0;
      }
    }
    else
    {
      if (abs(estatex - ustate[PIDx]) > 20)
      {
        correctstate[PIDx]++;
      }
    }
    if ((estatey < 180) && (estatey > (-180)) && ((abs(estatey - ustate[PIDy]) < 20 || correctstate[PIDy] > 50)))
    {
      ustate[PIDy] = estatey;
      if (correctstate[PIDy] != 0)
      {
        correctstate[PIDy] = 0;
      }
    }
    else
    {
      if (abs(estatey - ustate[PIDy]) > 20)
      {
        correctstate[PIDy]++;
      }
    }

  }
}

float Stoint(int beginnumber)
{
  float number;
  if (oneline[beginnumber] == '-')
  {
    if (oneline[beginnumber + 2] != '.')
    {
      if (oneline[beginnumber + 3] != '.')
      { number = (-1) * ((((int)oneline[beginnumber + 1] - 48) * 100) + (((int)oneline[beginnumber + 2] - 48) * 10) + (oneline[beginnumber + 3] - 48) + (((int)oneline[beginnumber + 5] - 48) * 0.1) + (((int)oneline[beginnumber + 6] - 48) * 0.01));
        dotlocation = beginnumber + 4;
      }
      else
      {
        number = (-1) * ((((int)oneline[beginnumber + 1] - 48) * 10) + ((int)oneline[beginnumber + 2] - 48) + (((int)oneline[beginnumber + 4] - 48) * 0.1) + (((int)oneline[beginnumber + 5] - 48) * 0.01));
        dotlocation = beginnumber + 3;
      }
    }
    else
    {
      number = (-1) * (((int)oneline[beginnumber + 1] - 48) + (((int)oneline[beginnumber + 3] - 48) * 0.1) + (((int)oneline[beginnumber + 4] - 48) * 0.01));
      dotlocation = beginnumber + 2;
    }
  }
  else
  {
    if (oneline[beginnumber + 1] != '.')
    {
      if (oneline[beginnumber + 2] != '.')
      { number = (((int)oneline[beginnumber] - 48) * 100) + (((int)oneline[beginnumber + 1] - 48) * 10) + (oneline[beginnumber + 2] - 48) + (((int)oneline[beginnumber + 4] - 48) * 0.1) + (((int)oneline[beginnumber + 5] - 48) * 0.01);
        dotlocation = beginnumber + 3;
      }
      else
      { number = (((int)oneline[beginnumber] - 48) * 10) + ((int)oneline[beginnumber + 1] - 48) + (((int)oneline[beginnumber + 3] - 48) * 0.1) + (((int)oneline[beginnumber + 4] - 48) * 0.01);
        dotlocation = beginnumber + 2;
      }
    }
    else
    {
      number = ((int)oneline[beginnumber] - 48) + (((int)oneline[beginnumber + 2] - 48) * 0.1) + (((int)oneline[beginnumber + 3] - 48) * 0.01);
      dotlocation = beginnumber + 1;
    }
  }

  return (number);
}


int getoneline()
{

  int slength = 0;
  while (Serial1.available() > 33)
  {
    Serial1.read();
  }
  while (Serial1.read() != '=')
  {
  }
  while (slength < 33)
  {
    oneline[slength] = Serial1.read();
    if (oneline[slength] == '0' || oneline[slength] == '1' || oneline[slength] == '2' || oneline[slength] == '3' || oneline[slength] == '4' || oneline[slength] == '5' || oneline[slength] == '6' || oneline[slength] == '7' || oneline[slength] == '8' || oneline[slength] == '9' || oneline[slength] == ',' || oneline[slength] == '.' || oneline[slength] == '-')
    {
      slength++;
    }
    else
    {
      slength--;
      return (slength);
    }
  }
}

float distance(int PIN_TRIG, int PIN_ECHO) //get the distance data
{
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  return pulseIn(PIN_ECHO, HIGH, 18000) / 58;
}

void averagedistance()
{
  float dis1, dis2, dis3, dis4;
  dis1 = distance(d1t, d1e);
  dis2 = distance(d2t, d2e);
  dis3 = distance(d3t, d3e);
  dis4 = distance(d4t, d4e);
  avd = (dis1 + dis2 + dis3 + dis4) / 4;
}


