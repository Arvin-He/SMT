// SMTDlg.h : header file
#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;


// CSMTDlg dialog
class CSMTDlg : public CDialogEx
{
// Construction
public:
	CSMTDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_SMT_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON		m_hIcon;
	CMenu		m_menu;
	CToolBar	m_toolBar;
	CImageList	m_imageList;

public:
	void ShowImage(Mat img, INT_PTR ID);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCamera_Open();
	afx_msg void OnCamera_Stop();
	afx_msg void OnCamera_Close();
	afx_msg void OnCamera_SavePic();
	afx_msg void OnCamera_SaveVideo();
	afx_msg void OnCamera_StopVideo();	
};
