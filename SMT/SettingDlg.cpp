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
	ON_BN_CLICKED(IDC_MODIFY_SOFTLIMIT_BTN, &CSettingDlg::OnClickedModifySoftlimitBtn)
	ON_BN_CLICKED(IDC_APPLY_SOFTLIMIT_BTN, &CSettingDlg::OnClickedApplySoftlimitBtn)
END_MESSAGE_MAP()



void CSettingDlg::OnClickedModifySoftlimitBtn()
{
	// TODO: Add your control notification handler code here
	 UpdateData(TRUE);
	 SetSoftLimitModifiable(IDC_STAGE_EL_UP_X_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_STAGE_EL_DOWN_X_EDIT,FALSE);
	 SetSoftLimitModifiable(IDC_STAGE_EL_UP_Y_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_STAGE_EL_DOWN_Y_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_CCD_EL_UP_X_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_CCD_EL_DOWN_X_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_CCD_EL_UP_Z_EDIT, FALSE);
	 SetSoftLimitModifiable(IDC_CCD_EL_DOWN_Z_EDIT, FALSE);
	 UpdateData(FALSE);
}

void CSettingDlg::SetSoftLimitModifiable(int nID, BOOL modifiable)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(nID);
	pEdit-> SetReadOnly(modifiable);
}

void CSettingDlg::OnClickedApplySoftlimitBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetSoftLimitModifiable(IDC_STAGE_EL_UP_X_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_STAGE_EL_DOWN_X_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_STAGE_EL_UP_Y_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_STAGE_EL_DOWN_Y_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_CCD_EL_UP_X_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_CCD_EL_DOWN_X_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_CCD_EL_UP_Z_EDIT, TRUE);
	SetSoftLimitModifiable(IDC_CCD_EL_DOWN_Z_EDIT, TRUE);
	UpdateData(FALSE);
}
