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
{

}

CManualDlg::~CManualDlg()
{
}

void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
END_MESSAGE_MAP()


// CManualDlg message handlers
BOOL CManualDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* pStageStepSizeComboBox = (CComboBox*)GetDlgItem(IDC_STAGE_STEPSIZE_COMBO);
	pStageStepSizeComboBox->AddString(_T("1"));
	pStageStepSizeComboBox->AddString(_T("2"));
	pStageStepSizeComboBox->AddString(_T("5"));
	pStageStepSizeComboBox->AddString(_T("10"));
	pStageStepSizeComboBox->AddString(_T("20"));
	pStageStepSizeComboBox->AddString(_T("50"));
	pStageStepSizeComboBox->AddString(_T("100"));
	pStageStepSizeComboBox->AddString(_T("200"));
	pStageStepSizeComboBox->SetCurSel(5);
	CComboBox* pStageSpeedComboBox = (CComboBox*)GetDlgItem(IDC_STAGE_SPEED_COMBO);
	pStageSpeedComboBox->AddString(_T("50"));
	pStageSpeedComboBox->AddString(_T("100"));
	pStageSpeedComboBox->AddString(_T("200"));
	pStageSpeedComboBox->AddString(_T("500"));
	pStageSpeedComboBox->AddString(_T("1000"));
	pStageSpeedComboBox->SetCurSel(2);
	CComboBox* pCCDStepSizeComboBox = (CComboBox*)GetDlgItem(IDC_CCD_STEPSIZE_COMBO);
	pCCDStepSizeComboBox->AddString(_T("1"));
	pCCDStepSizeComboBox->AddString(_T("2"));
	pCCDStepSizeComboBox->AddString(_T("5"));
	pCCDStepSizeComboBox->AddString(_T("10"));
	pCCDStepSizeComboBox->AddString(_T("20"));
	pCCDStepSizeComboBox->AddString(_T("50"));
	pCCDStepSizeComboBox->AddString(_T("100"));
	pCCDStepSizeComboBox->AddString(_T("200"));
	pCCDStepSizeComboBox->SetCurSel(5);
	CComboBox* pCCDSpeedComboBox = (CComboBox*)GetDlgItem(IDC_CCD_SPEED_COMBO);
	pCCDSpeedComboBox->AddString(_T("50"));
	pCCDSpeedComboBox->AddString(_T("100"));
	pCCDSpeedComboBox->AddString(_T("200"));
	pCCDSpeedComboBox->AddString(_T("500"));
	pCCDSpeedComboBox->AddString(_T("1000"));
	pCCDSpeedComboBox->SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CManualDlg::OnStageMove(UINT nID)
{
	CString strStepsize(_T(""));
	int stepSize(0);
	CComboBox *pStageStepSize = (CComboBox*)GetDlgItem(IDC_STAGE_STEPSIZE_COMBO);
	pStageStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);
	switch(nID)
	{
	case IDC_STAGE_XUP_BTN:
		
		break;
	case  IDC_STAGE_XDOWN_BTN:

		break;
	case IDC_STAGE_YUP_BTN:

		break;
	case  IDC_STAGE_YDOWN_BTN:

		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CManualDlg::OnCCDMove(UINT nID)
{
	CString strStepsize(_T(""));
	int stepSize(0);
	CComboBox *pStageStepSize = (CComboBox*)GetDlgItem(IDC_CCD_STEPSIZE_COMBO);
	pStageStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);
	switch(nID)
	{
	case IDC_CCD_XUP_BTN:

		break;
	case  IDC_CCD_XDOWN_BTN:

		break;
	case IDC_CCD_ZUP_BTN:

		break;
	case  IDC_CCD_ZDOWN_BTN:

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
	// �趨����ģʽ���߼����򣨴˴�����ģʽ�̶�ΪP+D��������+����	
	dmc_set_pulse_outmode(nCardNo, nAxisIndex, 0);
	// ���õ����˶��ٶ�����, m_nSpeedMin
	//dmc_set_profile(nCardNo, nAxisIndex, m_nSpeedMin, m_nSpeed, m_nAcc, m_nDec, 0);
	//�趨S��ʱ��
	//dmc_set_s_profile(nCardNo, nAxisIndex, 0, m_nSPara); //S ��ʱ�䣬��λ��s����Χ��0~0.5 s
	//�㶯(λ��ģʽ)
	dmc_pmove(nCardNo, nAxisIndex, nPulse*nDirection, nMoveMode);  //����0��ʾ����˶�
}

// ��װ���㺯��,����,���,���㷽��,�����ٶ�ģʽ,����ģʽ
void CManualDlg::DMC3000_GoHome(int nCardNo, int nAxisIndex, int nHomeDirection, int nHomeVelMode, int nHomeMode)
{
	//UpdateData(true);//ˢ�²���
	dmc_set_pulse_outmode(nCardNo, nAxisIndex, 0);  //�����������ģʽ
	//dmc_set_profile(nCardNo, nAxisIndex, m_nSpeedmin, m_nSpeedmax, m_nAcc, m_nDec, 500);//�����ٶ�����
	dmc_set_homemode(nCardNo, nAxisIndex, nHomeDirection, nHomeVelMode, nHomeMode, 1);//���û��㷽ʽ
	dmc_home_move(nCardNo, nAxisIndex);//���㶯��
	while (dmc_check_done(nCardNo, nAxisIndex)==0)      //�жϵ�ǰ��״̬
	{
// 		AfxGetApp()->PumpMessage();
// 		GetDlgItem(IDC_BUTTON1)->EnableWindow(false); 
	}
	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true); 
	//UpdateData(false);
}