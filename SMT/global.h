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
// 软限位设置参数
struct SoftLimitParam
{
	int nStageELUpX;
	int nStageELDownX;
	int nStageELUpY;
	int nStageELDownY;
	int nCCDELUpX;
	int nCCDELDownX;
	int nCCDELUpZ;
	int nCCDELDownZ;
};

// 
struct StageXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};

struct StageYAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};

struct CCDXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};

struct CCDZAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};

extern double TransPulseToDistance(int nAxisIndex, int nPulse);
extern int TransDistanceToPulse(int nAxisIndex, int nDistance);


