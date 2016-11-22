#pragma once


// CSetParamDlg dialog

class CSetParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetParamDlg)

public:
	CSetParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetParamDlg();

// Dialog Data
	enum { IDD = IDD_SETTING_SOFTWARE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
