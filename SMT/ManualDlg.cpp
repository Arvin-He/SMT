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
		dmc_pmove(0, 0, 50, 0);
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