#pragma once


// CErrorBox dialog

class CErrorBox : public CDialogEx
{
	DECLARE_DYNAMIC(CErrorBox)

public:
	CErrorBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CErrorBox();

	int m_dwStatus;
	void ReportError(int dwStatus);
// Dialog Data
	enum { IDD = IDD_ERROR_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
