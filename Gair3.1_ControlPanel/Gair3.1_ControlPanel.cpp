// Gair3.1_ControlPanel.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Gair3.1_ControlPanel.h"
#include "Camera.h"
#include "ThreadFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  

// Ψһ��Ӧ�ó������

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
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����:  MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  �ڴ˴�ΪӦ�ó������Ϊ��д���롣

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
		// TODO:  ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����:  GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
