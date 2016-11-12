#pragma once


// CImageAssistDlg dialog

class CImageAssistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageAssistDlg)

public:
	CImageAssistDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageAssistDlg();

// Dialog Data
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
