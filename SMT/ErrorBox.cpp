// ErrorBox.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "ErrorBox.h"
#include "afxdialogex.h"


// CErrorBox dialog

IMPLEMENT_DYNAMIC(CErrorBox, CDialogEx)

CErrorBox::CErrorBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CErrorBox::IDD, pParent)
{

}

CErrorBox::~CErrorBox()
{
}

void CErrorBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CErrorBox, CDialogEx)

	ON_BN_CLICKED(IDOK, &CErrorBox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CErrorBox::OnBnClickedCancel)
END_MESSAGE_MAP()


// CErrorBox message handlers


BOOL CErrorBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetActiveWindow();
	MessageBeep(0);
	ReportError(m_dwStatus);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CErrorBox::ReportError(int dwStatus)
{
	CString msg(_T(""));	
	msg.Format(_T("%s : 0x%08x"), _T("Error"), dwStatus);
	GetDlgItem(IDC_STATIC_ERROR_INFO)->SetWindowText(msg);
	UpdateWindow();
}

void CErrorBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CErrorBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
