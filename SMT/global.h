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
// 
extern struct StageXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
} stageXAxisParam;

extern struct StageYAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
} stageYAxisParam;

extern struct CCDXAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
} ccdXAxisParam;

extern struct CCDZAxisParam
{
	int nStartSpeed;
	int nRunSpeed;
	int nEndSpeed;
	double nAccelerateTime;
	double nDecelerateTime;
	double nSDecelerateTime;
} ccdZAxisParam;

extern double TransPulseToDistance(int nAxisIndex, int nPulse);
extern int TransDistanceToPulse(int nAxisIndex, int nDistance);


