// ManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "ManualDlg.h"
#include "afxdialogex.h"
#include "dmc3000/inc/LTDMC.h"
#include "global.h"

// CManualDlg dialog

IMPLEMENT_DYNAMIC(CManualDlg, CDialogEx)

CManualDlg::CManualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualDlg::IDD, pParent)
	, m_stageStep(1000)
	, m_ccdStep(1000)
{

}

CManualDlg::~CManualDlg()
{
}

void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STAGE_STEP_EDIT, m_stageStep);
	DDV_MinMaxInt(pDX, m_stageStep, 0, 100000);
	DDX_Text(pDX, IDC_CCD_STEP_EDIT, m_ccdStep);
	DDV_MinMaxInt(pDX, m_ccdStep, 0, 100000);
}


BEGIN_MESSAGE_MAP(CManualDlg, CDialogEx)
	ON_COMMAND_EX(IDC_STAGE_XUP_BTN, OnStageMove)
	ON_COMMAND_EX(IDC_STAGE_XDOWN_BTN, OnStageMove)
	ON_COMMAND_EX(IDC_STAGE_YUP_BTN, OnStageMove)
	ON_COMMAND_EX(IDC_STAGE_YDOWN_BTN, OnStageMove)
	ON_COMMAND_EX(IDC_CCD_XUP_BTN, OnCCDMove)
	ON_COMMAND_EX(IDC_CCD_XDOWN_BTN, OnCCDMove)
	ON_COMMAND_EX(IDC_CCD_ZDOWN_BTN, OnCCDMove)
	ON_COMMAND_EX(IDC_CCD_ZUP_BTN, OnCCDMove)
	ON_BN_CLICKED(IDC_INHALE_ON_BTN, &CManualDlg::OnClickedInhaleOnBtn)
	ON_BN_CLICKED(IDC_INHALE_OFF_BTN, &CManualDlg::OnClickedInhaleOffBtn)
END_MESSAGE_MAP()


// CManualDlg message handlers
BOOL CManualDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CManualDlg::OnStageMove(UINT nID)
{
	UpdateData(TRUE);
	switch(nID)
	{
	case IDC_STAGE_XUP_BTN:
		DMC3000_Move(0, 0, TransDistanceToPulse(0, m_stageStep), 1, 0);
		break;
	case  IDC_STAGE_XDOWN_BTN:
		DMC3000_Move(0, 0, TransDistanceToPulse(1, m_stageStep), -1, 0);
		break;
	case IDC_STAGE_YUP_BTN:
		DMC3000_Move(0, 1, TransDistanceToPulse(2, m_stageStep), 1, 0);
		break;
	case  IDC_STAGE_YDOWN_BTN:
		DMC3000_Move(0, 1, TransDistanceToPulse(3, m_stageStep), -1, 0);
		break;
	default:
		break;
	}
	UpdateData(FALSE);
	return TRUE;
}

BOOL CManualDlg::OnCCDMove(UINT nID)
{
	UpdateData(TRUE);

	switch(nID)
	{
	case IDC_CCD_XUP_BTN:
		DMC3000_Move(0, 2, TransDistanceToPulse(0, m_ccdStep), 1, 0);
		break;
	case  IDC_CCD_XDOWN_BTN:
		DMC3000_Move(0, 2, TransDistanceToPulse(0, m_ccdStep), -1, 0);
		break;
	case IDC_CCD_ZUP_BTN:
		DMC3000_Move(0, 3, TransDistanceToPulse(0, m_ccdStep), -1, 0);
		break;
	case  IDC_CCD_ZDOWN_BTN:
		DMC3000_Move(0, 3, TransDistanceToPulse(0, m_ccdStep), 1, 0);
		break;
	default:
		break;
	}
	UpdateData(FALSE);
	return TRUE;
}

// 封装dmc运动函数:卡号, 轴号, 脉冲数, 运动方向, 运动模式
void CManualDlg::DMC3000_Move(int nCardNo, int nAxisIndex, int nPulse, int nDirection, int nMoveMode)
{
	if (dmc_check_done(nCardNo, nAxisIndex) == 0) //已经在运动中
		return; 
	// 设置单轴运动速度曲线, m_nSpeedMin
	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);
	//设定S段时间
	dmc_set_s_profile(nCardNo, nAxisIndex, 0, 0.2); //S 段时间，单位：s；范围：0~0.5 s
	//点动(位置模式)
	dmc_pmove(nCardNo, nAxisIndex, nPulse*nDirection, nMoveMode);  //最后的0表示相对运动
}

// 封装回零函数,卡号,轴号,回零方向,回零速度模式,回零模式
void CManualDlg::DMC3000_GoHome(int nCardNo, int nAxisIndex, int nHomeDirection, int nHomeVelMode, int nHomeMode)
{
	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);//设置速度曲线
	dmc_set_homemode(nCardNo, nAxisIndex, nHomeDirection, nHomeVelMode, nHomeMode, 1);//设置回零方式
	dmc_home_move(nCardNo, nAxisIndex); //回零动作
	while (dmc_check_done(nCardNo, nAxisIndex)==0)      //判断当前轴状态
	{
// 		AfxGetApp()->PumpMessage();
// 		GetDlgItem(IDC_BUTTON1)->EnableWindow(false); 
		return;
	}
	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true); 
	//UpdateData(false);
}




void CManualDlg::OnClickedInhaleOnBtn()
{
	// TODO: Add your control notification handler code here
	dmc_write_outbit(g_nCardNo, 0, 1);
}


void CManualDlg::OnClickedInhaleOffBtn()
{
	// TODO: Add your control notification handler code here
	dmc_write_outbit(g_nCardNo, 0, 0);
}
