
bool fromremote()
{
  char commandFromRemote[18];
  uint8_t commandlength = 0;
  commandlength = receiveCommandFromRemote(commandFromRemote);
  if (commandlength == 11)
  {
    uint8_t modeTest = (uint8_t)commandFromRemote[1];
    float rstateXTest = floatStructor(2, commandFromRemote);
    float rstateYTest = floatStructor(4, commandFromRemote);
    float rstateZTest = floatStructor(6, commandFromRemote);
    uint8_t rtpowerTest = (uint8_t)commandFromRemote[8];

    if ((uint8_t)(rstateXTest + rstateYTest + rstateZTest + rtpowerTest + modeTest) == (uint8_t)commandFromRemote[9])
    {
      rstate[Xaxis] = rstateXTest;
      rstate[Yaxis] = rstateYTest;
      rstate[Zaxis] = rstateZTest;
      rtpower = rtpowerTest;
      mode = modeTest;
      decodeMode(mode, modes);
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

bool fromUAV()
{
  return 0;
}



uint8_t receiveCommandFromRemote(char command[18])
{
  int i = 1;
  if (Serial3.available() > 0)
  {
    command[0] = Serial3.read();
    while (Serial3.available() > 12 || command[0] != '$')
    {
      command[0] = Serial3.read();
    }

    while ( command[i - 1] != '&')
    {
      while (Serial3.available() == 0)//waiting for next byte
      {}
      command[i] = Serial3.read();
      i++;
    }
    return i;
  }
  return 0;
}

int receivecommandfromnav()
{
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
    modeSwitch[i] = (code << (8 - i)) >> 8;
  }
}
