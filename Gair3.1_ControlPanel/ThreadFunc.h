
#include "_COM.h"
#include <opencv2\opencv.hpp>

#ifndef ThreadFunc
#define ThreadFunc


#define CamraId 0
#define hscale 0.5
#define vscale 0.5
#define linewidth 1
#define ROI_h 200
#define ROI_w 140
#define Word_h 15
#define Word_w 16

Datas Data;
Camera cam = Camera(CamraId);
Mat camFrame,  UI;
CvScalar textColor = cvScalar(255, 255, 255);
bool exitThread;

Size screen(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));

DWORD WINAPI GetInfor(LPVOID pM)
{
	Data.remoteSignal = 0;
	Data.UAVSignal = 0;
	Data.commandCenterSignal = 0;
	while (!exitThread)
	{
		Data.commandCenterSignal = getData(Data);
		if (Data.commandCenterSignal)
		{
			//cout << 'x' << (float)Data.rstate[Xaxis] << 'y' << (float)Data.rstate[Yaxis] << 'z' << (float)Data.rstate[Zaxis] << 't' << (int)Data.tpower << '\n';
		}
	}

	return 0;
}

DWORD WINAPI ImProcessor(LPVOID pM)
{
	while (!exitThread)
	{

	}
	return 0;
}

DWORD WINAPI UIProcessor(LPVOID pm)
{

	char temp[100];
	CvFont UIMessageFont,simbolFont;
	cvInitFont(&UIMessageFont, CV_FONT_HERSHEY_SIMPLEX, hscale, vscale, 0, linewidth);
	cvInitFont(&simbolFont, CV_FONT_HERSHEY_TRIPLEX, hscale, vscale, 0, linewidth);
	Mat textROI;


	// blur(textROI, textROI, Size(3, 3));
	while (!exitThread)
	{
		if (cam.isOpen())
		{
			cam.getFrame(camFrame);
			textROI = camFrame(Rect(0, 0, ROI_w, ROI_h));
			Mat textMat(ROI_h, ROI_w, CV_8UC3, Scalar(0, 0, 0));
			if (Data.commandCenterSignal)
			{

				sprintf_s(temp, "Power:%3hd", Data.tpower);
				cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h), &UIMessageFont, textColor);


				if (Data.modes[stopMode])
				{
					sprintf_s(temp, "S");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w, Word_h * 2), &simbolFont, textColor);
				}
				else
				{
					sprintf_s(temp, "S");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w, Word_h * 2), &simbolFont, cvScalar(50, 50, 50));
				}
				if (Data.modes[keepMode])
				{
					sprintf_s(temp, "K");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 3, Word_h * 2), &simbolFont, textColor);
				}
				else
				{
					sprintf_s(temp, "K");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 3, Word_h * 2), &simbolFont, cvScalar(50, 50, 50));
				}
				if (Data.modes[landMode])
				{
					sprintf_s(temp, "L");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 5, Word_h * 2), &simbolFont, textColor);
				}
				else
				{
					sprintf_s(temp, "L");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 5, Word_h * 2), &simbolFont, cvScalar(50, 50, 50));
				}

				if (Data.modes[takeOffMode])
				{
					sprintf_s(temp, "T");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 7, Word_h * 2), &simbolFont, textColor);
				}
				else
				{
					sprintf_s(temp, "T");
					cvPutText(&IplImage(textMat), temp, cvPoint(Word_w * 7, Word_h * 2), &simbolFont, cvScalar(50, 50, 50));
				}

				line(textMat, cvPoint(0, Word_h * 2.5), cvPoint(ROI_w, Word_h * 2.5), textColor, 2, 8);


				sprintf_s(temp, "Remote:");
				cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 4), &UIMessageFont, textColor);

				if (Data.remoteSignal)
				{
					sprintf_s(temp, "State X:%3.2f", Data.rstate[Xaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 5.5), &UIMessageFont, textColor);
					sprintf_s(temp, "State Y:%3.2f", Data.rstate[Yaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 6.5), &UIMessageFont, textColor);
					sprintf_s(temp, "State Z:%3.2f", Data.rstate[Zaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 7.5), &UIMessageFont, textColor);
				}
				else
				{
					sprintf_s(temp, "No signal");
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 6), &UIMessageFont, textColor);
				}

				line(textMat, cvPoint(0, Word_h * 8), cvPoint(ROI_w, Word_h * 8), textColor, 2, 8);

				sprintf_s(temp, "UAV:");
				cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 9.5), &UIMessageFont, textColor);
				if (Data.UAVSignal)
				{
					sprintf_s(temp, "State X:%3.2f", Data.ustate[Xaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 11), &UIMessageFont, textColor);
					sprintf_s(temp, "State Y:%3.2f", Data.ustate[Yaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 12), &UIMessageFont, textColor);
					sprintf_s(temp, "State Z:%3.2f", Data.ustate[Zaxis]);
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 13), &UIMessageFont, textColor);
				}
				else
				{
					sprintf_s(temp, "No signal");
					cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 11.5), &UIMessageFont, textColor);
				}
			}
			else
			{
				sprintf_s(temp, "No device");
				cvPutText(&IplImage(textMat), temp, cvPoint(0, Word_h * 5), &UIMessageFont, textColor);
			}
			addWeighted(textROI, 0.6, textMat, 0.4, 50, textROI);
			resize(camFrame, UI, screen);
			waitKey(20);
		}
	}

	return 0;
}

#endif