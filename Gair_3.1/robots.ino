

bool takeoffrobot()//loop this function to up the UAV to 3m high
{
  fromremote();//get expect state data(state of remote)
  int timed = micros();
  dtimed = timed - btimed;
  btimed = timed;
  averagedistance();
  float errodistance = 300 - avd;
  float PIDpd = errodistance * Kpd;
  float PIDid = totalPIDid + errodistance * Kid;
  float PIDdd = (preverrodistance - errodistance) / dtimed * Kdd;
  preverrodistance = errodistance;
  int PIDd = PIDpd + PIDid + PIDdd;
  takeoffpower += PIDd;
  tpower = takeoffpower;
  rstate[PIDx] = 0;
  rstate[PIDy] = 0;

  realizestate();
}


void landrobot()
{
  fromremote();//get expect state data(state of remote)
  float distance1, distance2, distance3, distance4, errodistance;
  distance1 = distance(d1t, d1e);
  distance2 = distance(d2t, d2e);
  distance3 = distance(d3t, d3e);
  distance4 = distance(d4t, d4e);
  avd = (distance1 + distance2 + distance3 + distance4) / 4;
  if (distance1 == 0 || distance2 == 0 || distance3 == 0 || distance4 == 0)
  {
    if (tpower > 40 || distance1 < 200 || distance2 < 200 || distance3 < 200 || distance4 < 200)
    {
      tpower = 40;
    }
    else
    {
      Serial3.print("bre");//remind
    }
  }
  else
  {
    if (distance1 < 40 || distance2 < 40 || distance3 < 40 || distance4 < 40)
    {
      if (avd - distance1 > 5 || avd - distance2 > 5 || avd - distance3 || avd - distance4 > 5)
      {
        Serial3.print("bae");
        errodistance = 30 - avd;
      }
      else
      { errodistance = 0 - avd;
      }
    }
    else
    { errodistance = 0 - avd;
    }

    int timed = millis();
    dtimed = timed - btimed;
    btimed = timed;
    float PIDpd = errodistance * Kpd;
    float PIDid = totalPIDid + errodistance * Kid;
    float PIDdd = (preverrodistance - errodistance) / dtimed * Kdd;
    preverrodistance = errodistance;
    int PIDd = PIDpd + PIDid + PIDdd;
    landpower += PIDd;
    tpower = landpower;
    rstate[PIDx] = 0;
    rstate[PIDy] = 0;
  }
  realizestate();

}

void keeprobot()
{
  rstate[PIDx] = 0;
  rstate[PIDy] = 0;
  realizestate();
}

