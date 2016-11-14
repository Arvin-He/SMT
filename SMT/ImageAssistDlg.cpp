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
	m_bDrawCross  = FALSE;
	m_bDrawScale  = FALSE;
	m_bDrawLine   = FALSE;
	m_bDrawRect   = FALSE;
	m_bDrawCircle = FALSE;
	m_bMeasureDis = FALSE;
	m_bMeasureAngle = FALSE;
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
	ON_BN_CLICKED(IDC_DRAW_SCALE_BTN, &CImageAssistDlg::OnClickedDrawScaleBtn)
	ON_BN_CLICKED(IDC_DRAW_LINE_BTN, &CImageAssistDlg::OnClickedDrawLineBtn)
	ON_BN_CLICKED(IDC_DRAW_RECT_BTN, &CImageAssistDlg::OnClickedDrawRectBtn)
	ON_BN_CLICKED(IDC_DRAW_CIRCLE_BTN, &CImageAssistDlg::OnClickedDrawCircleBtn)
END_MESSAGE_MAP()


// CImageAssistDlg message handlers
void CImageAssistDlg::OnClickedShowCrossBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawCross)
	{
		m_bDrawCross = FALSE;
		GetDlgItem(IDC_SHOW_CROSS_BTN)->SetWindowText("隐藏十字");
	}
	else
	{
		m_bDrawCross =TRUE;
		GetDlgItem(IDC_SHOW_CROSS_BTN)->SetWindowText("显示十字");
	}
}


void CImageAssistDlg::OnClickedDrawScaleBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawScale)
	{
		m_bDrawScale = FALSE;
		GetDlgItem(IDC_DRAW_SCALE_BTN)->SetWindowText("隐藏比例尺");
	}
	else
	{
		m_bDrawScale =TRUE;
		GetDlgItem(IDC_DRAW_SCALE_BTN)->SetWindowText("显示比例尺");
	}
}


void CImageAssistDlg::OnClickedDrawLineBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawLine)
	{
		m_bDrawLine = FALSE;
		GetDlgItem(IDC_DRAW_LINE_BTN)->SetWindowText("隐藏直线");
	}
	else
	{
		m_bDrawLine =TRUE;
		GetDlgItem(IDC_DRAW_LINE_BTN)->SetWindowText("显示直线");
	}
}


void CImageAssistDlg::OnClickedDrawRectBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawRect)
	{
		m_bDrawRect = FALSE;
		GetDlgItem(IDC_DRAW_RECT_BTN)->SetWindowText("隐藏矩形框");
	}
	else
	{
		m_bDrawRect =TRUE;
		GetDlgItem(IDC_DRAW_RECT_BTN)->SetWindowText("显示矩形框");
	}
}


void CImageAssistDlg::OnClickedDrawCircleBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawCircle)
	{
		m_bDrawCircle = FALSE;
		GetDlgItem(IDC_DRAW_CIRCLE_BTN)->SetWindowText("隐藏圆形");
	}
	else
	{
		m_bDrawCircle =TRUE;
		GetDlgItem(IDC_DRAW_CIRCLE_BTN)->SetWindowText("显示圆形");
	}
}
