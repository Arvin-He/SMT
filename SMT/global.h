//////////////////////////////////////////////////////////////////////////
//������һЩȫ�ֱ���
//////////////////////////////////////////////////////////////////////////
#include <opencv2/core/core.hpp>
//ͼ��
extern cv::Mat g_src;    
//����
extern WORD g_nCardNo;  
//����
extern const int  g_nAxisCount;  
// ����λ���ò���
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

extern double TransPulseToDistance(int nAxisIndex, int nPulse);
extern int TransDistanceToPulse(int nAxisIndex, int nDistance);


