//////////////////////////////////////////////////////////////////////////
//������һЩȫ�ֱ���
//////////////////////////////////////////////////////////////////////////

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
//ͼ��
extern Mat g_src;    
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


