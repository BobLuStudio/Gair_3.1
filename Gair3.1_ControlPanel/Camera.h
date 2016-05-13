#include<opencv2\opencv.hpp>
using namespace cv;


class Camera
{
public:
	Camera(unsigned _int8 cameraId = 0)
	{
		setCameraId(cameraId);
	}
	bool isOpen()
	{
		return vedio.isOpened();
	}
	bool setCameraId(unsigned _int8 cameraId = 0)
	{
		vedio.open(cameraId);
		if (!vedio.isOpened())
		{
			printf("不能打开%u号摄像头设备！",cameraId);
			return 0;
		}
		return 1;
	}
	double frameWidth()
	{
		return  vedio.get(CV_CAP_PROP_FRAME_WIDTH);
	}
	double frameHight()
	{
		return  vedio.get(CV_CAP_PROP_FRAME_HEIGHT);
	}
	void getFrame(Mat &frame)
	{
		vedio.read(frame); 
	}
private:
	VideoCapture vedio;
};