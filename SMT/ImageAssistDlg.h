#pragma once
// CImageAssistDlg dialog
class CImageAssistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageAssistDlg)

public:
	CImageAssistDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageAssistDlg();
	BOOL	m_bDrawCross;
	BOOL	m_bDrawScale;
	BOOL    m_bDrawLine;
	BOOL	m_bDrawRect;
	BOOL    m_bDrawCircle;
	BOOL    m_bMeasureDis;
	BOOL    m_bMeasureAngle;
	void    *m_pSMTDlg;


// Dialog Data
	enum { IDD = IDD_IMAGE_ASSIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedShowCrossBtn();
	afx_msg void OnClickedDrawScaleBtn();
	afx_msg void OnClickedDrawLineBtn();
	afx_msg void OnClickedDrawRectBtn();
	afx_msg void OnClickedDrawCircleBtn();
	afx_msg void OnClickedMeasureDisBtn();
	afx_msg void OnClickedMeasureAngleBtn();
};
