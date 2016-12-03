//////////////////////////////////////////////////////////////////////////
//������һЩȫ�ֱ���
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
		// ���õ����˶��ٶ�����, m_nSpeedMin
		dmc_set_profile(g_nCardNo, 
			nAxisIndex, 
			g_stageXAxisParamStruct.nStartSpeed, 
			g_stageXAxisParamStruct.nRunSpeed,
			g_stageXAxisParamStruct.nAccelerateTime, 
			g_stageXAxisParamStruct.nDecelerateTime, 
			g_stageXAxisParamStruct.nEndSpeed);
		//�趨S��ʱ��
		dmc_set_s_profile(g_nCardNo, nAxisIndex, 0, g_stageXAxisParamStruct.nSDecelerateTime); //S ��ʱ�䣬��λ��s����Χ��0~0.5 s
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

// ��װdmc�˶�����:����, ���, ������, �˶�����, �˶�ģʽ
void DMC3000_Move(int nCardNo, int nAxisIndex, int nPulse, int nDirection, int nMoveMode)
{
	if (dmc_check_done(nCardNo, nAxisIndex) == 0) //�Ѿ����˶���
		return; 
	//�㶯(λ��ģʽ)
	dmc_pmove(nCardNo, nAxisIndex, nPulse*nDirection, nMoveMode);  //����0��ʾ����˶�
}

// ��װ���㺯��,����,���,���㷽��,�����ٶ�ģʽ,����ģʽ
// void CManualDlg::DMC3000_GoHome(int nCardNo, int nAxisIndex, int nHomeDirection, int nHomeVelMode, int nHomeMode)
// {
// 	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);//�����ٶ�����
// 	dmc_set_homemode(nCardNo, nAxisIndex, nHomeDirection, nHomeVelMode, nHomeMode, 1);//���û��㷽ʽ
// 	dmc_home_move(nCardNo, nAxisIndex); //���㶯��
// 	while (dmc_check_done(nCardNo, nAxisIndex)==0)      //�жϵ�ǰ��״̬
// 	{
// 		AfxGetApp()->PumpMessage();
// 		GetDlgItem(IDC_BUTTON1)->EnableWindow(false); 
// 		return;
// 	}
// 	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true); 
// 	//UpdateData(false);
// }