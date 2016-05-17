
bool fromremote()
{
  char commandFromRemote[11];
  if (receiveCommandFromRemote(commandFromRemote))
  {
    mode = (uint8_t)commandFromRemote[1];
    float rstateXTest = floatStructor(2, commandFromRemote);
    float rstateYTest = floatStructor(4, commandFromRemote);
    float rstateZTest = floatStructor(6, commandFromRemote);
    uint8_t rtpowerTest = (uint8_t)commandFromRemote[8];

    if ((uint8_t)(rstateXTest + rstateYTest + rstateZTest + rtpowerTest + mode) == (uint8_t)commandFromRemote[9])
    {
      rstate[Xaxis] = rstateXTest;
      rstate[Yaxis] = rstateYTest;
      rstate[Xaxis] = rstateZTest;
      tpower = rtpowerTest;
      decodeMode(mode, modes);
    }
    return 1;
  }
  return 0;
}


bool receiveCommandFromRemote(char command[11])
{
  if (Serial3.available() > 11)
  {
    command[0] = Serial3.read();
    uint8_t serialCatch = Serial3.available();
    while (serialCatch > 10 || command[0] != '$')
    {
      command[0] = Serial3.read();
      serialCatch = Serial3.available();
      if (serialCatch == 0)
      {
        if (command[0] != '$')
          return 0;
      }
    }

    for (uint16_t c = 0;  serialCatch < 10; c++) //waiting for data
    {
      serialCatch = Serial3.available();
      if (c > 20000)
      {
        return 0;
      }
    }

    for ( uint8_t i = 1; i <= 10; i++)
    {
      command[i] = Serial3.read();
    }
    if (command[10] == '&')
      return 1;
  }
  return 0;
}


float floatStructor(int beginNumber, char command[12]) //recive the data begin space
{
  int16_t temp = 0;
  temp = (uint8_t)command[beginNumber];
  temp |= (uint8_t)command[beginNumber + 1] << 8;
  return (float)temp / 100;
}

void decodeMode(uint8_t code, bool modeSwitch[4])
{
  for (uint8_t i = 0; i <= 3; i++)
 {
    modeSwitch[i] = (bool)((code & (1<<i))>>i);
  }
}

