//////////////////////////////////////////////////////////////////////////
//这里是一些全局变量
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SMT.h"
#include "global.h"

Mat g_src(Size(640, 480), CV_8UC3);
WORD g_nCardNo = 0;
const int g_nAxisCount = 4;

struct SoftLimitParam softLimitParam ={0, 0, 0, 0, 0, 0, 0, 0};


struct StageXAxisParam stageXAxisParam ={0, 1000, 100, 0.02, 0.02, 0.2};


struct StageYAxisParam stageYAxisParam = 
{
	stageXAxisParam.nStartSpeed = 0,
	stageXAxisParam.nRunSpeed = 1000,
	stageXAxisParam.nEndSpeed = 100,
	stageXAxisParam.nAccelerateTime = 0.02,
	stageXAxisParam.nDecelerateTime = 0.02,
	stageXAxisParam.nSDecelerateTime = 0.2
};

struct CCDXAxisParam ccdXAxisParam= 
{
	stageXAxisParam.nStartSpeed = 0,
	stageXAxisParam.nRunSpeed = 1000,
	stageXAxisParam.nEndSpeed = 100,
	stageXAxisParam.nAccelerateTime = 0.02,
	stageXAxisParam.nDecelerateTime = 0.02,
	stageXAxisParam.nSDecelerateTime = 0.2
};

struct CCDZAxisParam ccdZAxisParam =
{
	stageXAxisParam.nStartSpeed = 0,
	stageXAxisParam.nRunSpeed = 1000,
	stageXAxisParam.nEndSpeed = 100,
	stageXAxisParam.nAccelerateTime = 0.02,
	stageXAxisParam.nDecelerateTime = 0.02,
	stageXAxisParam.nSDecelerateTime = 0.2
};

double TransPulseToDistance(int nAxisIndex, int nPulse)
{
	switch(nAxisIndex)
	{
	case 0:
		return nPulse;
	case 1:
		return nPulse;
	case 2:
		return 2*nPulse;
	case 3:
		return 2*nPulse;
	default:
		break;
	}
	return 0;
}

int TransDistanceToPulse(int nAxisIndex, int nDistance)
{
	switch(nAxisIndex)
	{
	case 0:
		return nDistance;
	case 1:
		return nDistance;
	case 2:
		return nDistance/2;
	case 3:
		return nDistance/2;
	default:
		break;
	}
	return 0;
}