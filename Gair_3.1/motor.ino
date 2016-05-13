void motor(int sp1, float hpower)
{
  if (hpower > 100)  //correct power data
  {
    hpower = 100;
  }
  else if (hpower < 0)
  {
    hpower = 0;
  }

  int pwmlen = map(hpower, 0, 100, 900, 2100);
  powerServo[sp1].writeMicroseconds(pwmlen);
 
}

void applypowers()
{
  motor(fmpin, fpower); //use motor() to apply the power data
  motor(rmpin, rpower);
  motor(lmpin, lpower);
  motor(bmpin, bpower);
}



