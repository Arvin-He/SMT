//////////////////////////////////////////////////////////////////////////
//这里是一些全局变量
//////////////////////////////////////////////////////////////////////////
#include <opencv2/core/core.hpp>
//图像
extern cv::Mat g_src;    
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
extern SoftLimitParam g_softLimitParamStruct;
///////////////// 设置各轴的运动参数////////////
struct StageXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};
extern StageXAxisParam g_stageXAxisParamStruct;

struct StageYAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};
extern StageYAxisParam g_stageYAxisParamStruct;

struct CCDXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};
extern CCDXAxisParam g_CCDXAxisParamStruct;

struct CCDZAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
};
extern CCDZAxisParam g_CCDZAxisParamStruct;
///////////////// 设置各轴的运动参数////////////
extern double TransPulseToDistance(int nAxisIndex, int nPulse);
extern int TransDistanceToPulse(int nAxisIndex, int nDistance);
extern void SetMotionParam(int nAxisIndex);
extern void DMC3000_Move(int nCardNo, int nAxisIndex, int nPulse, int nDirection, int nMoveMode);

