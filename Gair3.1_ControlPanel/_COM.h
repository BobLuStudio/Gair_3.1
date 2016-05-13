
#include "COM.h"

#ifndef _COM_H
#define _COM_H

#define Xaxis 0
#define Yaxis 1
#define Zaxis 2

extern struct Datas
{
 float rstate[3], ustate[3];
 unsigned _int8 tpower=0, mode=0;
 bool modes[4];
 bool newInfor;
};

comPort Serial = comPort(0);


unsigned _int8 receiveCommandFromRemote(char* command)
{
	command[0] =Serial.read();
		for (int i=0; command[0] != '$'&&i <= 20;i++)
		{
			command[0] = Serial.read();
		}
		if (command[0] == '$')
		{
			for (unsigned _int8 i = 0; command[i] != '&'&&i <= 11;i++)
			{
				command[i+1] = Serial.read();
			}
			if (command[10] == '&')return 11;
		}
		return 0;
}


float floatStructor(int beginNumber, char* command) //recive the data begin space
{
	_int16 temp = 0;
	temp = (unsigned _int8)command[beginNumber];
	temp |= (unsigned _int8)command[beginNumber + 1] << 8;
	return (float)temp / 100;
}

void decodeMode(unsigned _int8 code, bool modeSwitch[4])
{
	for (unsigned _int8 i = 0; i <= 3; i++)
	{
		modeSwitch[i] = (bool)(code << (8 - i)) >> 8;
	}
}
bool getData(Datas &data)
{
	char commandFromRemote[12]; 
	unsigned _int8 commandlength = 0;
	commandlength = receiveCommandFromRemote(commandFromRemote);

	if (commandlength == 11)
	{
		unsigned _int8 modeTest = (unsigned _int8)commandFromRemote[1];
		float rstateXTest = floatStructor(2, commandFromRemote);
		float rstateYTest = floatStructor(4, commandFromRemote);
		float rstateZTest = floatStructor(6, commandFromRemote);
		unsigned _int8 rtpowerTest = (unsigned _int8)commandFromRemote[8];
		if ((unsigned _int8)(rstateXTest + rstateYTest + rstateZTest + rtpowerTest + modeTest) == (unsigned _int8)commandFromRemote[9])
		{
			if (rtpowerTest != 101)
			{
				data.rstate[Xaxis] = rstateXTest;
				data.rstate[Yaxis] = rstateYTest;
				data.rstate[Zaxis] = rstateZTest;
				data.tpower = rtpowerTest;
				data.mode = modeTest;
				decodeMode(data.mode, data.modes);
			}
			else
			{
				data.ustate[Xaxis] = rstateXTest;
				data.ustate[Yaxis] = rstateYTest;
				data.ustate[Zaxis] = rstateZTest;
				data.tpower = rtpowerTest;
				data.mode = modeTest;
				decodeMode(data.mode, data.modes);
			}
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

#endif