char oneline[28];
int dotlocation;

void reset()
{
  sensorx = 0;
  sensory = 0;
  state();
  while (abs(statex) > 20 || abs(statey) > 20)
  {
    digitalWrite(10, HIGH);
    state();
  }
  sensorx = 0 - statex;
  sensory = 0 - statey;
  digitalWrite(10, LOW);
}

void state()
{ dotlocation = 0;
  float estatex;
  if (Serial.available() > 0)
  {
    getoneline();
    statez = Stoint(dotlocation);
    statey = Stoint(dotlocation + 4) + sensory;
    estatex = Stoint(dotlocation + 4);
    if (estatex < 0)
    {
      statex = (-180) - estatex + sensorx;

    }
    else
    {
      statex = 180 - estatex + sensorx;
    }

    if (statex > 60 || statex < (-60) || statey > 60 || statey < (-60))
    {
      digitalWrite(10, HIGH);
    }
    else
    {
      digitalWrite(10, LOW);
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
  while (Serial.available() > 26)
  {
    Serial.read();
  }
  while (Serial.read() != '=')
  {
  }
  while (1)
  {
    oneline[slength] = Serial.read();
    if (oneline[slength] == '\n' || oneline[slength] == '\r' )
    {
      slength--;
      return (slength);
    }
    if (oneline[slength] == '0' || oneline[slength] == '1' || oneline[slength] == '2' || oneline[slength] == '3' || oneline[slength] == '4' || oneline[slength] == '5' || oneline[slength] == '6' || oneline[slength] == '7' || oneline[slength] == '8' || oneline[slength] == '9' || oneline[slength] == ',' || oneline[slength] == '.' || oneline[slength] == '-')
    {
      slength++;
    }
  }
}

float potentiometer()
{
  return (map(analogRead(A0), 3, 1020, 0, 100));
}


