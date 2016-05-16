// Gair3.1_ControlPanel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Gair3.1_ControlPanel.h"
#include "Camera.h"
#include "ThreadFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	int nRetCode = 0;

	HANDLE getInfor = CreateThread(NULL, 0, GetInfor, NULL, 0, NULL), 
		imProcessor = CreateThread(NULL, 0, ImProcessor, NULL, 0, NULL),
		UIOutput = CreateThread(NULL, 0, UIProcessor, NULL, 0, NULL);

	cam.getFrame(camFrame);
	camFrame.copyTo(UI);


	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。

			namedWindow("Gair UI-windows 3.2", CV_WINDOW_NORMAL);
			cv::setWindowProperty("Gair UI-windows 3.2", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);


			while (1)
			{
				if (cam.isOpen())
				{
					imshow("Gair UI-windows 3.2", UI);
					if (waitKey(20) == 27)
					{
						exitThread = 1; 
						cam.~Camera();
						Serial.~comPort();
						return nRetCode;
					}
				}
			}
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
