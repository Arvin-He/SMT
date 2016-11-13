// ManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "ManualDlg.h"
#include "afxdialogex.h"
#include "dmc3000/inc/LTDMC.h"

// CManualDlg dialog

IMPLEMENT_DYNAMIC(CManualDlg, CDialogEx)

CManualDlg::CManualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualDlg::IDD, pParent)
	, m_stageStepEdit(1000)
	, m_ccdStepEdit(1000)
{

}

CManualDlg::~CManualDlg()
{
}

void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STAGE_STEP_EDIT, m_stageStepEdit);
	DDV_MinMaxInt(pDX, m_stageStepEdit, 0, 100000);
	DDX_Text(pDX, IDC_CCD_STEP_EDIT, m_ccdStepEdit);
	DDV_MinMaxInt(pDX, m_ccdStepEdit, 0, 100000);
	DDX_Control(pDX, IDC_STAGE_X_CHECK, m_stageXCheck);
	DDX_Control(pDX, IDC_STAGE_Y_CHECK, m_stageYCheck);
	DDX_Control(pDX, IDC_CCD_X_CHECK, m_ccdXCheck);
	DDX_Control(pDX, IDC_CCD_Z_CHECK, m_ccdYCheck);
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
	ON_BN_CLICKED(IDC_STAGE_GOHOME_BTN, &CManualDlg::OnClickedStageGohomeBtn)
	ON_BN_CLICKED(IDC_CCD_GOHOME_BTN, &CManualDlg::OnClickedCcdGohomeBtn)
	
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
	switch(nID)
	{
	case IDC_STAGE_XUP_BTN:
		DMC3000_Move(0, 0, m_stageStepEdit, 1, 0);
		break;
	case  IDC_STAGE_XDOWN_BTN:
		DMC3000_Move(0, 0, m_stageStepEdit, -1, 0);
		break;
	case IDC_STAGE_YUP_BTN:
		DMC3000_Move(0, 1, m_stageStepEdit, 1, 0);
		break;
	case  IDC_STAGE_YDOWN_BTN:
		DMC3000_Move(0, 1, m_stageStepEdit, -1, 0);
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CManualDlg::OnCCDMove(UINT nID)
{
	switch(nID)
	{
	case IDC_CCD_XUP_BTN:
		DMC3000_Move(0, 2, m_ccdStepEdit, 1, 0);
		break;
		break;
	case  IDC_CCD_XDOWN_BTN:
		DMC3000_Move(0, 2, m_ccdStepEdit, -1, 0);
		break;
		break;
	case IDC_CCD_ZUP_BTN:
		DMC3000_Move(0, 3, m_ccdStepEdit, -1, 0);
		break;
	case  IDC_CCD_ZDOWN_BTN:
		DMC3000_Move(0, 3, m_ccdStepEdit, 1, 0);
		break;
	default:
		break;
	}
	return TRUE;
}

// ��װdmc�˶�����:����, ���, ������, �˶�����, �˶�ģʽ
void CManualDlg::DMC3000_Move(int nCardNo, int nAxisIndex, int nPulse, int nDirection, int nMoveMode)
{
	if (dmc_check_done(nCardNo, nAxisIndex) == 0) //�Ѿ����˶���
		return; 
	// ���õ����˶��ٶ�����, m_nSpeedMin
	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);
	//�趨S��ʱ��
	dmc_set_s_profile(nCardNo, nAxisIndex, 0, 0.2); //S ��ʱ�䣬��λ��s����Χ��0~0.5 s
	//�㶯(λ��ģʽ)
	dmc_pmove(nCardNo, nAxisIndex, nPulse*nDirection, nMoveMode);  //����0��ʾ����˶�
}

// ��װ���㺯��,����,���,���㷽��,�����ٶ�ģʽ,����ģʽ
void CManualDlg::DMC3000_GoHome(int nCardNo, int nAxisIndex, int nHomeDirection, int nHomeVelMode, int nHomeMode)
{
	dmc_set_profile(nCardNo, nAxisIndex, 100, 1000, 0.02, 0.02, 100);//�����ٶ�����
	dmc_set_homemode(nCardNo, nAxisIndex, nHomeDirection, nHomeVelMode, nHomeMode, 1);//���û��㷽ʽ
	dmc_home_move(nCardNo, nAxisIndex); //���㶯��
// 	while (dmc_check_done(nCardNo, nAxisIndex)==0)      //�жϵ�ǰ��״̬
// 	{
// // 		AfxGetApp()->PumpMessage();
// // 		GetDlgItem(IDC_BUTTON1)->EnableWindow(false); 
// 	}
	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true); 
	//UpdateData(false);
}

void CManualDlg::OnClickedStageGohomeBtn()
{
	// TODO: Add your control notification handler code here
	DMC3000_GoHome(0, 0, 1, 0, 0);
	//dmc_home_move(0, 0); //���㶯��
	// dmc_pmove(0, 0, 0, 1);
// 	if (m_stageXCheck.GetCheck())
// 	{
// 		DMC3000_GoHome(0, 0, )
// 	}
// 	if (m_stageYCheck.GetCheck())
// 	{
// 
// 	}
	
	//if (dmc_check_done(0, nAxisIndex) == 0) //�Ѿ����˶���
		return; 
}


void CManualDlg::OnClickedCcdGohomeBtn()
{
	// TODO: Add your control notification handler code here
}


