#pragma once


// CSetParamDlg dialog

class CSetParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetParamDlg)

public:
	CSetParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetParamDlg();

// Dialog Data
	enum { IDD = IDD_SETTING_PARAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetMotionParamModifiable(int nID, BOOL modifiable);
	void InitMotionParam();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedModifyMotionParamBtn();
	afx_msg void OnBnClickedApplyMotionParamBtn();
	virtual BOOL OnInitDialog();
};
