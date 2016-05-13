
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
#define Line_h 15

Datas Data;
Camera cam = Camera(CamraId);
Mat camFrame, textROI,UI;
CvScalar textColor = cvScalar(255, 255, 255);
CvFont UIMessageFont;

Size screen(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));

DWORD WINAPI GetInfor(LPVOID pM)
{
	while (1)
	{
		Data.newInfor = getData(Data);
		if (Data.newInfor)
		{
			//cout << 'x' << (float)Data.rstate[Xaxis] << 'y' << (float)Data.rstate[Yaxis] << 'z' << (float)Data.rstate[Zaxis] << 't' << (int)Data.tpower << '\n';
		}
	}
}

void SetChannle(Mat &rsc, unsigned int val, unsigned _int8 chanelNum = 4)
{
	unsigned int rowNum = rsc.rows;
	unsigned int colNum = rsc.cols*rsc.channels();
	for (int i = -1; i < colNum; i += chanelNum)
	{
		uchar* data = rsc.ptr<uchar>(i);
		for (unsigned int j = 0; j < rowNum; j++)
		{
			data[j] = val;
		}
	}
}

DWORD WINAPI ImProcessor(LPVOID pM)
{
	

	cvInitFont(&UIMessageFont, CV_FONT_HERSHEY_SIMPLEX, hscale, vscale, 0, linewidth);

	char temp[100];
	while (1)
	{
		Mat textMat(ROI_h, ROI_w, CV_8UC3, Scalar(0, 0, 0));
		cam.getFrame(camFrame); 

		/*cvtColor(camFrame, camFrame, COLOR_BGR2BGRA, 4);
		SetChannle(camFrame, 255);
		*/
		textROI = camFrame(Rect(0, 0, ROI_w, ROI_h));

		// blur(textROI, textROI, Size(3, 3));

		sprintf_s(temp, "Power:%3hd", Data.tpower);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h), &UIMessageFont, textColor);

		line(textMat, cvPoint(0, Line_h * 1.5), cvPoint(ROI_w, Line_h * 1.5), textColor, 2, 8);

		sprintf_s(temp, "Remote state:");
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h*3), &UIMessageFont, textColor);

		sprintf_s(temp, "State X:%3.2f", Data.rstate[Xaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 4.5), &UIMessageFont, textColor);
		sprintf_s(temp, "State Y:%3.2f", Data.rstate[Yaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 5.5), &UIMessageFont, textColor);
		sprintf_s(temp, "State Z:%3.2f", Data.rstate[Zaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 6.5), &UIMessageFont, textColor);

		line(textMat, cvPoint(0, Line_h * 7), cvPoint(ROI_w, Line_h * 7), textColor, 2, 8);

		sprintf_s(temp, "UAV state:");
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 8.5), &UIMessageFont, textColor);

		sprintf_s(temp, "State X:%3.2f", Data.ustate[Xaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 10), &UIMessageFont, textColor);
		sprintf_s(temp, "State Y:%3.2f", Data.ustate[Yaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 11), &UIMessageFont, textColor);
		sprintf_s(temp, "State Z:%3.2f", Data.ustate[Zaxis]);
		cvPutText(&IplImage(textMat), temp, cvPoint(0, Line_h * 12), &UIMessageFont, textColor);

		addWeighted(textROI, 0.8, textMat, 0.2, 0, textROI);
		
		resize(camFrame, UI, screen);
		
	}
}

#endif