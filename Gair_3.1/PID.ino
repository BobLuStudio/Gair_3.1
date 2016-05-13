void realizestate()
{
  if (tpower > 1)
  {
    state();//get state data of UAV
    btime = time; //note the time between loops
    time = micros();
    dtime = (time - btime);
    if (dtime < 0)
    {
      dtime = (time - 0) + (4294967295 - btime);
    }
    // PID
    PID(PIDx);
    PID(PIDy);
    PID(PIDz);
    PIDout[PIDz] = 0;

    fpower = tpower + PIDout[PIDz];
    bpower = fpower;
    lpower = tpower - PIDout[PIDz];
    rpower = lpower;

    //power balance

    if (fpower < 0)
    {
      int errofpower = 0 - fpower;
      fpower = 0;
      bpower = 0;
      if (lpower + errofpower < 100)
      {
        lpower = lpower + errofpower;
        rpower = lpower;
      }
      else
      {
        lpower = 100;
        rpower = 100;
      }
    }
    else
    {
      if (fpower > 100)
      {
        int errofpower = fpower - 100;
        fpower = 100;
        bpower = 100;
        if (lpower > errofpower)
        {
          lpower = lpower - errofpower;
          rpower = lpower;
        }
        else
        {
          lpower = 0;
          rpower = 0;
        }
      }
    }

    if (lpower < 0)
    {
      int errolpower = 0 - lpower;
      lpower = 0;
      rpower = 0;
      if (fpower + errolpower < 100)
      {
        fpower = fpower + errolpower;
        bpower = fpower;
      }
      else
      {
        fpower = 100;
        bpower = 100;
      }
    }
    else
    {
      if (lpower > 100)
      {
        int errolpower = lpower - 100;
        lpower = 100;
        rpower = 100;
        if (fpower > errolpower)
        {
          fpower = fpower - errolpower;
          bpower = fpower;
        }
        else
        {
          fpower = 0;
          bpower = 0;
        }
      }
    }
    fpower = tpower + PIDout[PIDx];
    bpower = tpower - PIDout[PIDx];
    lpower = tpower + PIDout[PIDy];
    rpower = tpower - PIDout[PIDy];

    if (fpower < 0)
    {
      int errofpower = 0 - fpower;
      fpower = 0;
      if (bpower + errofpower < 100)
      {
        bpower = bpower + errofpower;
      }
      else
      {
        bpower = 100;
      }
    }
    else
    {
      if (fpower > 100)
      {
        int errofpower = fpower - 100;
        fpower = 100;
        if (bpower > errofpower)
        {
          bpower = bpower - errofpower;
        }
        else
        {
          bpower = 0;
        }
      }
    }

    if (bpower < 0)
    {
      int errobpower = 0 - bpower;
      bpower = 0;
      if (fpower + errobpower < 100)
      {
        fpower = fpower + errobpower;
      }
      else
      {
        fpower = 100;
      }
    }
    else
    {
      if (bpower > 100)
      {
        int errobpower = bpower - 100;
        bpower = 100;
        if (fpower > errobpower)
        {
          fpower = fpower - errobpower;
        }
        else
        {
          fpower = 0;
        }
      }
    }

    if (lpower < 0)
    {
      int errolpower = 0 - lpower;
      lpower = 0;
      if (rpower + errolpower < 100)
      {
        rpower = rpower + errolpower;
      }
      else
      {
        rpower = 100;
      }
    }
    else
    {
      if (lpower > 100)
      {
        int errolpower = lpower - 100;
        lpower = 100;
        if (rpower > errolpower)
        {
          rpower = rpower - errolpower;
        }
        else
        {
          rpower = 0;
        }
      }
    }

    if (rpower < 0)
    {
      int errorpower = 0 - rpower;
      rpower = 0;
      if (lpower + errorpower < 100)
      {
        lpower = lpower + errorpower;
      }
      else
      {
        lpower = 100;
      }
    }
    else
    {
      if (rpower > 100)
      {
        int errorpower = rpower - 100;
        rpower = 100;
        if (lpower > errorpower)
        {
          lpower = lpower - errorpower;
        }
        else
        {
          lpower = 0;
        }
      }
    }
    applypowers();
  }
  else
  {
    motor(fmpin, 0);
    motor(rmpin, 0);
    motor(lmpin, 0);
    motor(bmpin, 0);
  }
}

void PID(int PIDNUM)
{
  *(PIDe[PIDNUM] + 1) = *PIDe[PIDNUM];//store previous PID error

  *PIDe[PIDNUM] = rstate[PIDNUM] - ustate[PIDNUM];//Get nowtime PID error

  if (dtime != 0)
    Acceleration[PIDNUM] = ((*PIDe[PIDNUM]) - (* (PIDe[PIDNUM] + 1))) / dtime * 1000000;

  float OUTPIDtotal;
  float OUTpout, OUTiout, OUTdout; //each output part of P,I,D
  //**********************************************//P(out)
  OUTpout = *PIDe[PIDNUM] * (*OUTPID[PIDNUM]);
  //**********************************************//I(out)
  /*
    OUTiout = OUTPIDi[PIDNUM] + *PIDe[PIDNUM] * (*(OUTPID[PIDNUM] + 1)) / 100;
    if (OUTiout > OUTPIDiMAX[PIDNUM])
    {
    OUTiout = OUTPIDiMAX[PIDNUM];
    }
    OUTPIDi[PIDNUM] = OUTiout;
  */
  //**********************************************//D(out)
  OUTdout = Acceleration[PIDNUM] * (*(OUTPID[PIDNUM] + 2));
  //**********************************************//Total(out)
  OUTPIDtotal = OUTpout + OUTiout + OUTdout;


  //**********************************************//INNER PID
  *(PIDeAcc[PIDNUM] + 1) = *PIDeAcc[PIDNUM];//back up
  *PIDeAcc[PIDNUM] = OUTPIDtotal - Acceleration[PIDNUM];
  double Pout, Iout, Dout; //each output part of PID
  //**********************************************//P
  Pout = *PIDeAcc[PIDNUM] * (*INNERPID[PIDNUM]) / 10;
  //**********************************************//I
  Iout = PIDi[PIDNUM] + *PIDeAcc[PIDNUM] * (*(INNERPID[PIDNUM] + 1)) / 10;
  if (Iout > PIDiMAX[PIDNUM])
  {
    Iout = PIDiMAX[PIDNUM];
  }
  PIDi[PIDNUM] = OUTiout;
  //**********************************************//D
  Dout = (*PIDeAcc[PIDNUM] - * (PIDeAcc[PIDNUM] + 1)) / dtime * (*(INNERPID[PIDNUM] + 2)) / 10;


  PIDout[PIDNUM] = Pout + Iout + Dout;

  if ( PIDout[PIDNUM] - prevPID[PIDNUM] > PIDDifferenceMAX[PIDNUM])
  {
    PIDout[PIDNUM] = prevPID[PIDNUM] + PIDDifferenceMAX[PIDNUM];
  } else
  {
    if (prevPID[PIDNUM] - PIDout[PIDNUM] > PIDDifferenceMAX[PIDNUM])
    {
      PIDout[PIDNUM] = prevPID[PIDNUM] - PIDDifferenceMAX[PIDNUM];
    }
  }
  prevPID[PIDNUM] = PIDout[PIDNUM];

  /*
    if (PIDout[PIDNUM] > tpower * kPIDMAX)
    {
    PIDout[PIDNUM] = tpower * kPIDMAX;
    }
    if (PIDout[PIDNUM] < (-1 * (tpower * kPIDMAX)))
    {
    PIDout[PIDNUM] = (-1 * (tpower * kPIDMAX));
    }
  */
}
