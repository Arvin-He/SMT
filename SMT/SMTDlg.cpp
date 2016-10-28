// SMTDlg.cpp : implementation file
#include "stdafx.h"
#include "SMT.h"
#include "SMTDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSMTDlg dialog
CSMTDlg::CSMTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSMTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_OPEN, &CSMTDlg::OnCamera_Open)	
	ON_COMMAND(IDM_STOP, &CSMTDlg::OnCamera_Stop)
	ON_COMMAND(IDM_CLOSE, &CSMTDlg::OnCamera_Close)
	ON_COMMAND(IDM_SAVE_PIC, &CSMTDlg::OnCamera_SavePic)
	ON_COMMAND(IDM_SAVE_VIDEO, &CSMTDlg::OnCamera_SaveVideo)
	ON_COMMAND(IDM_STOP_VIDEO, &CSMTDlg::OnCamera_StopVideo)
END_MESSAGE_MAP()


// CSMTDlg message handlers

BOOL CSMTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	ShowWindow(SW_MAXIMIZE);
	// TODO: Add extra initialization here
	SetWindowText(_T("贴片机系统"));
	//add menu
	if (!m_menu.LoadMenu(IDR_MENU))
	{
		TRACE0("菜单加载失败！");
		return -1;
	}
	SetMenu(&m_menu);
	//add toolBar
	if (!m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(4,4,0,0))
		|| !m_toolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("工具栏加载失败！");
		return -1;
	}
	//add toolbar icon
	m_imageList.Create(32, 32, ILC_MASK|ILC_COLOR24, 1, 1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_OPEN));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_STOP));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_CLOSE));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_SAVE_PIC));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_SAVE_VIDEO));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_STOP_VIDEO));
	m_toolBar.GetToolBarCtrl().SetImageList(&m_imageList);
	m_toolBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_imageList.Detach();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMTDlg::OnPaint()
{
	if (IsIconic()){
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSMTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSMTDlg::OnCamera_Open()
{

}

void CSMTDlg::OnCamera_Stop()
{

}

void CSMTDlg::OnCamera_Close()
{

}

void CSMTDlg::OnCamera_SavePic()
{

}

void CSMTDlg::OnCamera_SaveVideo()
{

}

void CSMTDlg::OnCamera_StopVideo()
{

}

void CSMTDlg::ShowImage(Mat img, INT_PTR ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect(0,0,0,0);
	GetDlgItem(ID)->GetClientRect(&rect);
	int tx = (int)(rect.Width() - img.cols)/2;
	int ty = (int)(rect.Height() - img.rows)/2;
	SetRect(rect, tx, ty, tx+img.cols, ty+img.rows);
	CvvImage cimg;
	IplImage temp = img;
	cimg.CopyOf(&temp);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}