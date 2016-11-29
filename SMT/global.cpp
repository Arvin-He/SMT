//////////////////////////////////////////////////////////////////////////
//这里是一些全局变量
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SMT.h"
#include "global.h"

cv::Mat g_src(cv::Size(640, 480), CV_8UC3);
WORD g_nCardNo = 0;
const int g_nAxisCount = 4;

struct SoftLimitParam g_softLimitParamStruct = {1000, 0, 1000, 0, 700, 0, 700, 0};

struct StageXAxisParam g_stageXAxisParamStruct = {0, 1000, 100, 0.02, 0.02, 0.2};

struct StageYAxisParam g_stageYAxisParamStruct = {0, 1000, 100, 0.02, 0.02, 0.2};

struct CCDXAxisParam g_CCDXAxisParamStruct = {0, 1000, 100, 0.02, 0.02, 0.2};

struct CCDZAxisParam g_CCDZAxisParamStruct = {0, 1000, 100, 0.02, 0.02, 0.2};

double TransPulseToDistance(int nAxisIndex, int nPulse)
{
	if (nAxisIndex == 0)
	{
		return nPulse;
	}
	else if (nAxisIndex == 1)
	{
		return nPulse;
	}
	else if (nAxisIndex == 2)
	{
		return 2*nPulse;
	}
	else if (nAxisIndex == 3)
	{
		return 2*nPulse;
	}
	else
	{
		return -1;
	}
}

int TransDistanceToPulse(int nAxisIndex, int nDistance)
{
	if (nAxisIndex == 0)
	{
		return nDistance;
	}
	else if (nAxisIndex == 1)
	{
		return nDistance;
	}
	else if (nAxisIndex == 2)
	{
		return nDistance/2;
	}
	else if (nAxisIndex == 3)
	{
		return nDistance/2;
	}
	else
	{
		return -1;
	}
}