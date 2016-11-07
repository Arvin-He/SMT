#pragma once


// CSemiAutoDlg dialog

class CSemiAutoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSemiAutoDlg)

public:
	CSemiAutoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSemiAutoDlg();

// Dialog Data
	enum { IDD = IDD_SEMI_AUTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
