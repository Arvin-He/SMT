//////////////////////////////////////////////////////////////////////////
//这里是一些全局变量
//////////////////////////////////////////////////////////////////////////

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
//图像
extern Mat g_src;    
//卡号
extern WORD g_nCardNo;  
//轴数
extern const int  g_nAxisCount;  
// 
extern struct
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	int nAccelerateTime;
	int nDecelerateTime;
	int nSDecelerateTime;
};

extern double TransPulseToDistance(int nAxisIndex, int nPulse);
extern int TransDistanceToPulse(int nAxisIndex, int nDistance);


