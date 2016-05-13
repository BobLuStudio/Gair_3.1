
#include <iostream>  
#ifndef COM_H
#define COM_H
#define InQueue 20
#define OutQueue 20

using namespace std;


class comPort
{
public:
	comPort(int comNum)
	{
		do
		{
			intToCOM(comNum, COMName);
			cout << COMName.c_str()<<'\n';
			port = CreateFile(COMName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			comNum++;
		}
		while (port == INVALID_HANDLE_VALUE && comNum<99); 
		if (port != INVALID_HANDLE_VALUE)
		{
			comAttributes.BaudRate = 57600;
			comAttributes.fParity = 0;
			comAttributes.ByteSize = 8;
			comAttributes.Parity = NOPARITY;
			comAttributes.StopBits = ONESTOPBIT;

			TimeOut.ReadIntervalTimeout = 0;
			TimeOut.ReadTotalTimeoutMultiplier = 1000 / 57600 * 8;
			TimeOut.WriteTotalTimeoutMultiplier = TimeOut.ReadTotalTimeoutMultiplier;
			TimeOut.ReadTotalTimeoutConstant = 100;
			TimeOut.WriteTotalTimeoutConstant = 100;

			SetCommState(port, &comAttributes);
			SetupComm(port, InQueue, OutQueue);
			SetCommTimeouts(port, &TimeOut);
			PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR);
			working = 1;
		}

	}
	~comPort()
	{
		CloseHandle(port);
	}

	unsigned _int8 available()
	{
		COMSTAT ComStat;
		DWORD dwErrorFlags;
		ClearCommError(port, &dwErrorFlags, &ComStat);
		return 1;//ComStat.cbInQue;
	}

	unsigned _int8 read()
	{
		if (working)
		{
			unsigned _int8 rst;
			DWORD wCount = 0;
			ReadFile(port, &rst, 1, &wCount, NULL);
			if (wCount == 1)
			{
				return rst;
			}
		}
			return NULL;
	}

	bool read(string &rsc, unsigned _int8 wRequist = 1)
	{
		char temp[12];
		if (working )
		{
			DWORD wCount = 0;
			ReadFile(port, temp, wRequist, &wCount, NULL);
			cout << temp;
			if (wCount == wRequist)
			{
				rsc.assign(temp);
				return 1;
			}
		}
		return NULL;
	}
	BOOL write(char rsc)
	{
		if (working)
		{
			DWORD dwBytesWrite;
			return WriteFile(port, &rsc, 1, &dwBytesWrite, NULL);
		}
	}

	/*bool begin(DWORD speed, unsigned _int8 config)
	{
	comAttributes.BaudRate = speed;
	}*/
private:
	bool intToCOM(int num, string &rst)
	{
		if (num >= 10)
		{
			rst.assign("\\\\.\\COM");
			rst += (char)(num / 10 + '0');
			rst += (char)(num % 10 + '0');
		}
		else
		{
			rst.assign("COM");
			rst += (char)(num + '0');
		}
		return 1;
	}

	HANDLE port;
	char readData[23];

	HANDLE portmWatchThread;
	DCB comAttributes;
	_COMMTIMEOUTS TimeOut;
	bool working = 0;
	string COMName;
};

#endif