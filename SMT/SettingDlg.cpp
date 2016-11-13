// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "dmc3000/inc/LTDMC.h"

// CSettingDlg dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SET_STAGE_ORIGIN_X_BTN, &CSettingDlg::OnClickedSetStageOriginXBtn)
	ON_BN_CLICKED(IDC_SET_STAGE_ORIGIN_Y_BTN, &CSettingDlg::OnClickedSetStageOriginYBtn)
	ON_BN_CLICKED(IDC_SET_CCD_ORIGIN_X_BTN, &CSettingDlg::OnClickedSetCcdOriginXBtn)
	ON_BN_CLICKED(IDC_SET_CCD_ORIGIN_Z_BTN, &CSettingDlg::OnClickedSetCcdOriginZBtn)
END_MESSAGE_MAP()


// CSettingDlg message handlers
void CSettingDlg::OnClickedSetStageOriginXBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_home_pin_logic(0, 0, 1);
	dmc_set_homemode(0, 0, 0, 0, 0);
	dmc_set_homelatch_mode(0, 0, 1, 0, 0);
	dmc_set_position(0, 0, 0);
}


void CSettingDlg::OnClickedSetStageOriginYBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_home_pin_logic(0, 1, 1);
}


void CSettingDlg::OnClickedSetCcdOriginXBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_home_pin_logic(0, 2, 1);
}


void CSettingDlg::OnClickedSetCcdOriginZBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_home_pin_logic(0, 3, 1);
}