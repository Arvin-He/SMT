// ImageAssistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "ImageAssistDlg.h"
#include "afxdialogex.h"
#include "dmc3000/inc/LTDMC.h"
#include "global.h"

// CImageAssistDlg dialog

IMPLEMENT_DYNAMIC(CImageAssistDlg, CDialogEx)

CImageAssistDlg::CImageAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageAssistDlg::IDD, pParent)
{

}

CImageAssistDlg::~CImageAssistDlg()
{
}

void CImageAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageAssistDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_SHOW_CROSS_BTN, &CImageAssistDlg::OnClickedShowCrossBtn)
END_MESSAGE_MAP()


// CImageAssistDlg message handlers

// »­Ê®×Ö
void CImageAssistDlg::DrawCross(Mat img)
{
	int crossLen = 20;
	Point centerPt(320, 240);
	circle(img, centerPt, 15, Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x-20, centerPt.y), 
		Point(centerPt.x+20, centerPt.y), Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x, centerPt.y-20), 
		Point(centerPt.x, centerPt.y+20), Scalar(0,255,0), 1, CV_AA);
}

// »­±ÈÀý³ß
void CImageAssistDlg::DrawImgScale(Mat img)
{
	rectangle(img, Point(568, 490), Point(630, 493), Scalar(0,255,0), -1, CV_AA);
	putText(img, "400um", Point(574, 485), FONT_HERSHEY_COMPLEX_SMALL, 0.6, Scalar(0,255,0), 1, CV_AA);
}

// void CImageAssistDlg::DrawLine(Mat img)
// {
// 	if (m_bDrawLine && (m_LinePtNum == 2))
// 		line(img, m_LinePt[0], m_LinePt[1], Scalar(255, 255, 0), 2, CV_AA);
// }
// 
// void CImageAssistDlg::DrawRect(Mat img)
// {
// 	if (m_bDrawRect && m_RectPtNum == 3)
// 	{
// 		line(img, m_RectPt[0], m_RectPt[1], Scalar(255, 255, 0), 2, CV_AA);
// 		line(img, m_RectPt[1], m_RectPt[2], Scalar(255, 255, 0), 2, CV_AA);
// 		line(img, m_RectPt[2], Point(m_RectPt[0].x+(m_RectPt[2].x-m_RectPt[1].x), 
// 			m_RectPt[2].y+(m_RectPt[0].y-m_RectPt[1].y)), Scalar(255, 255, 0), 2, CV_AA);
// 		line(img, Point(m_RectPt[0].x+(m_RectPt[2].x-m_RectPt[1].x), 
// 			m_RectPt[2].y+(m_RectPt[0].y-m_RectPt[1].y)), m_RectPt[0], Scalar(255, 255, 0), 2, CV_AA);
// 	}
// }
// 
// void CImageAssistDlg::DrawCircle(Mat img)
// {
// 	if (m_bDrawCircle && m_CirclePtNum == 2)
// 	{
// 		int disX = abs(m_CirclePt[1].x-m_CirclePt[0].x);
// 		int disY = abs(m_CirclePt[1].y-m_CirclePt[0].y);
// 		double radius = sqrt((double)(disX*disX) + (double)(disY*disY));
// 		circle(img, m_CirclePt[0], (int)radius, Scalar(0, 255, 255), 2, CV_AA);
// 	}
// }

void CImageAssistDlg::OnClickedShowCrossBtn()
{
	// TODO: Add your control notification handler code here
	DrawCross(g_src);
}
