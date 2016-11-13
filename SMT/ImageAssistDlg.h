#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

// CImageAssistDlg dialog

class CImageAssistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageAssistDlg)

public:
	CImageAssistDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageAssistDlg();
	void DrawCross(Mat img);
	void DrawImgScale(Mat img);

// Dialog Data
	enum { IDD = IDD_IMAGE_ASSIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnClickedShowCrossBtn();
};
