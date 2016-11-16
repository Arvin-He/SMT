// ImageAssistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMT.h"
#include "ImageAssistDlg.h"
#include "afxdialogex.h"
#include "dmc3000/inc/LTDMC.h"
#include "global.h"
#include "SMTDlg.h"
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
	ON_BN_CLICKED(IDC_MEASURE_DIS_BTN, &CImageAssistDlg::OnClickedMeasureDisBtn)
	ON_BN_CLICKED(IDC_MEASURE_ANGLE_BTN, &CImageAssistDlg::OnClickedMeasureAngleBtn)
END_MESSAGE_MAP()


// CImageAssistDlg message handlers
void CImageAssistDlg::OnClickedShowCrossBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawCross)
	{
		m_bDrawCross = FALSE;
		GetDlgItem(IDC_SHOW_CROSS_BTN)->SetWindowText("����ʮ��");
	}
	else
	{
		m_bDrawCross =TRUE;
		GetDlgItem(IDC_SHOW_CROSS_BTN)->SetWindowText("��ʾʮ��");
	}
}


void CImageAssistDlg::OnClickedDrawScaleBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawScale)
	{
		m_bDrawScale = FALSE;
		GetDlgItem(IDC_DRAW_SCALE_BTN)->SetWindowText("���ر�����");
	}
	else
	{
		m_bDrawScale =TRUE;
		GetDlgItem(IDC_DRAW_SCALE_BTN)->SetWindowText("��ʾ������");
	}
}


void CImageAssistDlg::OnClickedDrawLineBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawLine)
	{
		m_bDrawLine = FALSE;
		CSMTDlg* pSMTDlg = (CSMTDlg*)m_pSMTDlg;
		pSMTDlg->m_drawLinePoints.clear();
		GetDlgItem(IDC_DRAW_LINE_BTN)->SetWindowText("����ֱ��");
	}
	else
	{
		m_bDrawLine =TRUE;
		GetDlgItem(IDC_DRAW_LINE_BTN)->SetWindowText("��ʾֱ��");
	}
}


void CImageAssistDlg::OnClickedDrawRectBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawRect)
	{
		m_bDrawRect = FALSE;
		CSMTDlg* pSMTDlg = (CSMTDlg*)m_pSMTDlg;
		pSMTDlg->m_drawRectPoints.clear();
		GetDlgItem(IDC_DRAW_RECT_BTN)->SetWindowText("���ؾ��ο�");
	}
	else
	{
		m_bDrawRect =TRUE;
		GetDlgItem(IDC_DRAW_RECT_BTN)->SetWindowText("��ʾ���ο�");
	}
}


void CImageAssistDlg::OnClickedDrawCircleBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bDrawCircle)
	{
		m_bDrawCircle = FALSE;
		CSMTDlg* pSMTDlg = (CSMTDlg*)m_pSMTDlg;
		pSMTDlg->m_drawCirclePoints.clear();
		GetDlgItem(IDC_DRAW_CIRCLE_BTN)->SetWindowText("����Բ��");
	}
	else
	{
		m_bDrawCircle =TRUE;
		GetDlgItem(IDC_DRAW_CIRCLE_BTN)->SetWindowText("��ʾԲ��");
	}
}


void CImageAssistDlg::OnClickedMeasureDisBtn()
{
	// TODO: Add your control notification handler code here
	if (m_bMeasureDis)
	{
		m_bMeasureDis = FALSE;
		CSMTDlg* pSMTDlg = (CSMTDlg*)m_pSMTDlg;
		pSMTDlg->m_drawCirclePoints.clear();
		//GetDlgItem(IDC_MEASURE_DIS_BTN)->SetWindowText("����Բ��");
	}
	else
	{
		m_bMeasureDis =TRUE;
		//GetDlgItem(IDC_DRAW_CIRCLE_BTN)->SetWindowText("��ʾԲ��");
	}

}


void CImageAssistDlg::OnClickedMeasureAngleBtn()
{
	// TODO: Add your control notification handler code here
}
