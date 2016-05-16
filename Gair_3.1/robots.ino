

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
  float distance1, distance2, distance3, distance4, errodistance;
  distance1 = distance(d1t, d1e);
  distance2 = distance(d2t, d2e);
  distance3 = distance(d3t, d3e);
  distance4 = distance(d4t, d4e);
  avd = (distance1 + distance2 + distance3 + distance4) / 4;

  rstate[PIDx] = 0;
  rstate[PIDy] = 0;
  
  realizestate();
}

void keeprobot()
{
  rstate[PIDx] = 0;
  rstate[PIDy] = 0;
  realizestate();
}

