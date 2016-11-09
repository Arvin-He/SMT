#pragma once


// CManualDlg dialog

class CManualDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualDlg)

public:
	CManualDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualDlg();
	BOOL OnStageMove(UINT nID);
	BOOL OnCCDMove(UINT nID);
// Dialog Data
	enum { IDD = IDD_MANUAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
