//////////////////////////////////////////////////////////////////////////
//这里是一些全局变量
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SMT.h"
#include "global.h"
#include "dmc3000/inc/LTDMC.h"

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

void SetMotionParam(int nAxisIndex)
{
	if (nAxisIndex == 0)
	{
		// 设置单轴运动速度曲线, m_nSpeedMin
		dmc_set_profile(g_nCardNo, 
			nAxisIndex, 
			g_stageXAxisParamStruct.nStartSpeed, 
			g_stageXAxisParamStruct.nRunSpeed,
			g_stageXAxisParamStruct.nAccelerateTime, 
			g_stageXAxisParamStruct.nDecelerateTime, 
			g_stageXAxisParamStruct.nEndSpeed);
		//设定S段时间
		dmc_set_s_profile(g_nCardNo, nAxisIndex, 0, g_stageXAxisParamStruct.nSDecelerateTime); //S 段时间，单位：s；范围：0~0.5 s
	}
	else if (nAxisIndex == 1)
	{
		dmc_set_profile(g_nCardNo, 
			nAxisIndex, 
			g_stageYAxisParamStruct.nStartSpeed, 
			g_stageYAxisParamStruct.nRunSpeed,
			g_stageYAxisParamStruct.nAccelerateTime, 
			g_stageYAxisParamStruct.nDecelerateTime, 
			g_stageYAxisParamStruct.nEndSpeed);
		dmc_set_s_profile(g_nCardNo, nAxisIndex, 0, g_stageYAxisParamStruct.nSDecelerateTime);
	}
	else if (nAxisIndex == 2)
	{
		dmc_set_profile(g_nCardNo, 
			nAxisIndex, 
			g_CCDXAxisParamStruct.nStartSpeed, 
			g_CCDXAxisParamStruct.nRunSpeed,
			g_CCDXAxisParamStruct.nAccelerateTime, 
			g_CCDXAxisParamStruct.nDecelerateTime, 
			g_CCDXAxisParamStruct.nEndSpeed);
		dmc_set_s_profile(g_nCardNo, nAxisIndex, 0, g_CCDXAxisParamStruct.nSDecelerateTime);
	}
	else if (nAxisIndex == 3)
	{
		dmc_set_profile(g_nCardNo, 
			nAxisIndex, 
			g_CCDZAxisParamStruct.nStartSpeed, 
			g_CCDZAxisParamStruct.nRunSpeed,
			g_CCDZAxisParamStruct.nAccelerateTime, 
			g_CCDZAxisParamStruct.nDecelerateTime, 
			g_CCDZAxisParamStruct.nEndSpeed);
		dmc_set_s_profile(g_nCardNo, nAxisIndex, 0, g_CCDZAxisParamStruct.nSDecelerateTime);
	}
}

// 封装dmc运动函数:卡号, 轴号, 脉冲数, 运动方向, 运动模式
void DMC3000_Move(int nCardNo, int nAxisIndex, int nPulse, int nDirection, int nMoveMode)
{
	if (dmc_check_done(nCardNo, nAxisIndex) == 0) //已经在运动中
		return; 
	//点动(位置模式)
	dmc_pmove(nCardNo, nAxisIndex, nPulse*nDirection, nMoveMode);  //最后的0表示相对运动
}

// 封装回零函数,卡号,轴号,回零方向,回零速度模式,回零模式
// void CManualDlg::DMC3000_GoHome(int nCardNo, int nAxisIndex, int nHomeDirection, int nHomeVelMode, int nHomeMode)
// {
// 	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);//设置速度曲线
// 	dmc_set_homemode(nCardNo, nAxisIndex, nHomeDirection, nHomeVelMode, nHomeMode, 1);//设置回零方式
// 	dmc_home_move(nCardNo, nAxisIndex); //回零动作
// 	while (dmc_check_done(nCardNo, nAxisIndex)==0)      //判断当前轴状态
// 	{
// 		AfxGetApp()->PumpMessage();
// 		GetDlgItem(IDC_BUTTON1)->EnableWindow(false); 
// 		return;
// 	}
// 	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true); 
// 	//UpdateData(false);
// }