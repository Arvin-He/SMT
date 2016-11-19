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


