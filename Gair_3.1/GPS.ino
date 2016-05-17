
struct _UTC
{
  uint8_t hour,minute,second,date,mounth,year;
};


struct _GPS
{
  public:
  bool GetInform()
  {
  }
  char* ReadData()
  {
  }
  struct _UTC UTC;
  uint8_t speed_Knots;
  uint16_t direction_North;
  int32_t longitude,attitude;//in 0.5s
  
  private:
  void Get_GPRMC_Frame()
  {
  }
}GPS;
