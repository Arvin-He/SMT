﻿// SMTDlg.cpp : implementation file
#include "stdafx.h"
#include "SMT.h"
#include "SMTDlg.h"
#include "afxdialogex.h"
#include "ErrorBox.h"
#include "CvvImage.h"
#include "dmc3000/inc/LTDMC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MAX
#define MAX(a,b)      (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)      (((a) < (b)) ? (a) : (b))
#endif

//定义自己的0
#define  MY_ZERO 0.000000001
//const
const HV_RESOLUTION Resolution          = RES_MODE0;
const HV_SNAP_MODE SnapMode             = CONTINUATION;
const HV_BAYER_CONVERT_TYPE ConvertType = BAYER2RGB_NEIGHBOUR1;
const long Gain               = 10;
const long ExposureTint_Upper = 60;
const long ExposureTint_Lower = 1000;
const long ADCLevel           = ADC_LEVEL2;
const int XStart              = 0;
const int YStart              = 0;
const int Width               = 640;
const int Height              = 512;
const HV_SNAP_SPEED SnapSpeed = HIGH_SPEED;

// CSMTDlg dialog
CSMTDlg::CSMTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMTDlg::IDD, pParent)
	, m_editShutter(600)
	, m_editGain(4)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_src = Mat(Size(640, 512), CV_8UC3);
	InitialDHCamera();
	m_bIsCapture = FALSE;
}

void CSMTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_sliderGain);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_spinGain);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER, m_sliderShutter);
	DDX_Control(pDX, IDC_SPIN_SHUTTER, m_spinShutter);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_editShutter);
	DDV_MinMaxInt(pDX, m_editShutter, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_editGain);
	DDV_MinMaxInt(pDX, m_editGain, 0, 63);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CSMTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SNAP_CHANGE, OnSnapChange)	
	ON_MESSAGE(WM_SNAP_ERROR, OnSnapError)
	ON_MESSAGE(WM_SNAP_STOP, OnSnapStop)
	ON_COMMAND(IDM_OPEN, &CSMTDlg::OnCamera_Open)	
	ON_COMMAND(IDM_STOP, &CSMTDlg::OnCamera_Stop)
	ON_COMMAND(IDM_CLOSE, &CSMTDlg::OnCamera_Close)
	ON_COMMAND(IDM_SAVE_PIC, &CSMTDlg::OnCamera_SavePic)
	ON_COMMAND(IDM_SAVE_VIDEO, &CSMTDlg::OnCamera_SaveVideo)
	ON_COMMAND(IDM_STOP_VIDEO, &CSMTDlg::OnCamera_StopVideo)
	ON_WM_HSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CSMTDlg::OnTcnSelchangeTab)
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

	m_spinGain.SetRange(0, 63);
	m_spinGain.SetPos(m_editGain);
	m_spinGain.SetBuddy(GetDlgItem(IDC_EDIT_GAIN));
	m_sliderGain.SetRange(0, 63);
	m_sliderGain.SetPos(m_editGain);
	m_sliderGain.SetTicFreq(6);

	m_spinShutter.SetRange(0, 100);
	m_spinShutter.SetPos(m_editShutter);
	m_spinShutter.SetBuddy(GetDlgItem(IDC_EDIT_SHUTTER));
	m_sliderShutter.SetRange(0,100);
	m_sliderShutter.SetPos(m_editShutter);
	m_sliderShutter.SetTicFreq(10);

	m_tab.InsertItem(0, _T("手动控制"));
	m_tab.InsertItem(1, _T("自动控制"));
	m_manualDlg.Create(IDD_MANUAL, GetDlgItem(IDC_TAB));
	m_semiAutoDlg.Create(IDD_SEMI_AUTO, GetDlgItem(IDC_TAB));
	m_manualDlg.EnableWindow(TRUE);
	m_semiAutoDlg.EnableWindow(TRUE);

	CRect mainDlgRect(0,0,0,0);
	GetClientRect(&mainDlgRect);

	mainDlgRect.top += 600;
	mainDlgRect.left += 10;
	mainDlgRect.bottom -= 10;
	mainDlgRect.right -= 10;
	m_tab.MoveWindow(&mainDlgRect);
	//CRect tabRect(600, 10, 500, 300);
	//m_tab.MoveWindow(&tabRect);

	CRect tabRect(0,0,0,0);
	m_tab.GetClientRect(&tabRect);

	tabRect.top += 20;
	tabRect.bottom -= 10;
	tabRect.left += 10;
	tabRect.right -= 10;
	m_manualDlg.MoveWindow(&tabRect);
	m_semiAutoDlg.MoveWindow(&tabRect);
	//CRect pageRect(620, 20, 500, 300);
	//m_manualDlg.MoveWindow(&pageRect);
	//m_semiAutoDlg.MoveWindow(&pageRect);
	m_manualDlg.ShowWindow(SW_SHOW);
	m_semiAutoDlg.ShowWindow(SW_HIDE);
	m_tab.SetCurSel(0);


	InitDMC3000Card();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

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

HCURSOR CSMTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////CCD函数///////////////////////////
//获取bayer格式
HV_BAYER_LAYOUT CSMTDlg::HVGetBayerType(HHV hhv) 
{
	int nSize = 0; 
	HVSTATUS status = STATUS_OK;;
	HV_BAYER_LAYOUT Layout;
	status = HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, NULL, &nSize);	          
	if(STATUS_OK != status){
		Layout = BAYER_GR;
	}
	else{
		BYTE *pbBayerLayout = NULL;
		pbBayerLayout = new BYTE[nSize];
		status = HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, pbBayerLayout,&nSize);
		Layout = (HV_BAYER_LAYOUT)*pbBayerLayout;
		delete []pbBayerLayout; 
		pbBayerLayout=NULL;
	}
	return Layout;
}

/*	函数:SetExposureTime
	输入参数:int nWindWidth			当前图像宽度		
			 int lTintUpper			曝光时间的分子，shutter的取值
			 int lTintLower			曝光时间的分母，与shutter的单位相关	（ms:1000；s:1）
	输出参数:无						
	说明:设置曝光时间（其他的参数如摄像机时钟频率，消隐值都取默认值）*/
HVSTATUS CSMTDlg::SetExposureTime(HHV hhv, int nWindWidth, long lTintUpper, long lTintLower, 
								  long HBlanking, HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution)
{
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(hhv, DESC_DEVICE_TYPE, &type, &size);	
	int nOutputWid = nWindWidth;
	double dExposure = 0.0;
	double dTint = MAX((double)lTintUpper/(double)lTintLower, MY_ZERO);
	double lClockFreq = 0.0;  
	lClockFreq = (SnapSpeed == HIGH_SPEED)? 24000000:12000000;
	long lTb = HBlanking;
	lTb += 9;
	lTb -= 19;	
	if(lTb <= 0) 
		lTb =0;
	if(((double)nOutputWid + 244.0 + lTb ) > 552)	
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 244.0 + lTb);
	else
		dExposure = ((dTint * lClockFreq)+ 180.0) / 552 ;
	if((dExposure-(int)dExposure) >0.5)
		dExposure += 1.0;
	if(dExposure <= 0)
		dExposure = 1;
	else if(dExposure > 16383) 
		dExposure = 16383;
	return HVAECControl(hhv, AEC_EXPOSURE_TIME, (long)dExposure);
}

/*	函数:OnSnapError；
	输入参数:WPARAM wParam 没有使用	,LPARAM lParam 异常状态码；
    输出参数:LRESULT；说明:摄像机采集异常错误报告*/
LRESULT CSMTDlg::OnSnapError(WPARAM wParam, LPARAM lParam)
{	
	CErrorBox ErrDlg;
	ErrDlg.m_dwStatus = lParam;
	if (ErrDlg.m_dwStatus == dwLastStatus){
		if (ErrDlg.DoModal()==IDOK){
			OnCamera_Stop();
			OnCamera_Open();  
		}
	}
 	return 1;
}

LRESULT CSMTDlg::OnSnapStop(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart = FALSE;
	return TRUE;
}

BOOL CSMTDlg::DestroyDHCamera(HHV hDHCamera, BYTE *pRawBuffer, BYTE *pImageBuffer)
{
	HVSTATUS status = STATUS_OK;
	//	关闭数字摄像机，释放数字摄像机内部资源
	status = EndHVDevice(hDHCamera);
	HV_VERIFY(status);
	//	回收图像缓冲区
	delete []pRawBuffer;
	delete []pImageBuffer;
	return TRUE;
}

HVSTATUS CSMTDlg::GetLastStatus()
{
	HV_ARG_GET_LAST_STATUS ArgFeature;
	ArgFeature.type = HV_LAST_STATUS_TRANSFER;	
	HV_RES_GET_LAST_STATUS ResFeature;	
	HVAPI_CONTROL_PARAMETER  p;
	p.pInBuf		 = &ArgFeature;
	p.dwInBufSize	 = sizeof(ArgFeature);	
	p.pOutBuf		 = &ResFeature;
	p.dwOutBufSize	 = sizeof(ResFeature);
	p.pBytesRet		 = NULL;		
	p.code			 = ORD_GET_LAST_STATUS;
	int	dwSize = sizeof(p);
	HVSTATUS status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, &dwSize);
	if(status != STATUS_OK)
	{
		TRACE("HVCommand return status is %d", status);
	}
	dwLastStatus = 0;
	if (HV_SUCCESS(status))
		dwLastStatus = ResFeature.status;
	return status;
}

/*	函数:SnapThreadCallback
	输入参数:SNAP_INFO *pInfo；SNAP_INFO结构包括当前数字摄像机SNAP执行状态；输出参数:int						
	说明:数字摄像机采集到内存回调函数，但用户一般不用调用，由用户提供给SDK使用，
	用户在回调函数内实现对采集数据的处理和显示即可*/
int CALLBACK CSMTDlg::SnapThreadCallback(HV_SNAP_INFO *pInfo)
{
	CSMTDlg *This = (CSMTDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	HVSTATUS status = This->GetLastStatus();	// 读取摄像机图像采集工作状态	
	if(!HV_SUCCESS(status)) //如果函数调用不成功，摄像机停止图像采集。
	{	
		::PostMessage(hwnd, WM_SNAP_STOP, 0, 0);
		return 1;
	}
	TRACE("LastStatus is %X", This->dwLastStatus);
	if(This->dwLastStatus == 0)	// 当状态值等于0时，表示摄像机图像采集正常，则调用SendMessage显示图像；
	{
	   /*发送自定义消息WM_SNAP_EX_CHANGE到主窗口，同时传入当前可以处理的图像序号
		*注意：用SendMessage发送消息，必须等待消息处理完毕后，才能退出整个SendMessage函数*/
		::PostMessage(hwnd, WM_SNAP_CHANGE, 0, 0);
		return 1;	
	}
	else if(This->dwLastStatus == STATUS_FILE_INVALID)// 当状态值不等于-18时，表示摄像机发生1次丢帧行为,在连续高速采集时，1次丢帧行为可能包括1帧以上图像。
	{
		This->m_nLost++;	//丢帧计数
	}
	else if (This->dwLastStatus == 0xc0000012 || This->dwLastStatus == 0xc0000011)
	{
		This->m_nError++;	//错误计数
	}
	else // 当状态值为其他非0值时，表示摄像机图像采集异常
	{
	   //发送自定义消息 WM_SNAP_ERROR 到主窗口，注意：用PostMessage发送消息，不必等待消息处理完，就可以返回。
		::PostMessage(hwnd, WM_SNAP_ERROR, 0, This->dwLastStatus);
	}	
	return 1;
}

/*	函数:OnSnapChange1
	输入参数:WPARAM wParam	字参数，在消息中为当前可以处理的图像序号; LPARAM lParam	没有使用；
	输出参数:LRESULT；说明:实现对采集数据的处理和显示*/
LRESULT CSMTDlg::OnSnapChange(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status = STATUS_OK;	
	//	将原始图像数据进行Bayer转换，转换后为24位。同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer, Width, Height, ConvertType, m_pLutR, m_pLutG, m_pLutB, false, m_Layout);	
	m_src.data = (uchar*)m_pImageBuffer;
	flip(m_src, m_src, -1);
// 	if (m_bDrawCross1)
// 		DrawCross(g_src1);
// 	if (m_bCalibrateRectH && m_bDrawCalibrateRectH)
// 		DrawCalibrateRectH(g_src1);
// 	if (m_bMeasureSrc1)
// 		MeasureSrc1(g_src1);
// 	if (m_bSetImgScaleH)
// 		SetImgScale(g_src1);

	ShowImage(m_src, IDC_SHOW_PIC);	
	return 1;
}

/*	初始化数字摄像机硬件状态，用户也可以在其他位置初始化数字摄像机，但应保证数字摄像机已经打开，
	建议用户在应用程序初始化时，同时初始化数字摄像机硬件。 */
BOOL CSMTDlg::InitialDHCamera()
{
	//	初始化所有成员变量，同时打开数字摄像机
	HVSTATUS status = STATUS_OK;	
	m_bOpen		    = FALSE;
	m_bStart		= FALSE;
	m_pBmpInfo		= NULL;
	m_pRawBuffer	= NULL;
	m_pImageBuffer	= NULL;
	m_lHBlanking	= 0;
	m_lVBlanking	= 0;
	m_nError		= 0;
	m_nLost			= 0;
	for(int i=0; i<256; i++)
	{
		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;
	}
	//	打开数字摄像机 1
	status = BeginHVDevice(1, &m_hhv);
	//	检验函数执行状态，如果失败，则返回错误状态消息框
	HV_VERIFY(status);
	m_Layout = HVGetBayerType(m_hhv);
	//	设置数字摄像机分辨率
	HVSetResolution(m_hhv, Resolution);		
	//	采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)
	HVSetSnapMode(m_hhv, SnapMode);	
	//  设置各个分量的增益
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
	//  设置ADC的级别
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);
	//  获取设备类型
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv, DESC_DEVICE_TYPE, &type, &size);	
	//设置消隐。
	HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
	/*视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，视频窗口左上角X坐标和窗口宽度应为4的倍数，
	左上角Y坐标和窗口高度应为2的倍数，输出窗口的起始位置一般设置为(0, 0)即可。*/
	HVSetOutputWindow(m_hhv, XStart, YStart, Width, Height);	
	//设置采集速度
	HVSetSnapSpeed(m_hhv, SnapSpeed);
	//设置曝光时间
	SetExposureTime(m_hhv, Width, ExposureTint_Upper, ExposureTint_Lower, m_lHBlanking, SnapSpeed, Resolution);	
	//	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo					= (BITMAPINFO *)m_chBmpBuf;
	//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo->bmiHeader.biSize	= sizeof(BITMAPINFOHEADER);
	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth	= Width;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo->bmiHeader.biHeight	= Height;	
	/*	以下设置一般相同，对于低于8位的位图，还应设置相应的位图调色板*/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression	    = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed		    = 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;	
	/*	分配原始图像缓冲区，一般用来存储采集图像原始数据
	*  一般图像缓冲区大小由输出窗口大小和视频格式确定。*/
	m_pRawBuffer = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer);
	//分配Bayer转换后图像数据缓冲
	m_pImageBuffer = new BYTE[Width * Height * 3];
	ASSERT(m_pImageBuffer);
	return TRUE;
}
////////////////////////////CCD的函数//////////////////////////////////////////////

void CSMTDlg::OnCamera_Open()
{
	HVSTATUS status = STATUS_OK;
	//初始化数字摄像机采集图像到内存的控制，指定回调函数SnapThreadCallback和this指针
	status = HVOpenSnap(m_hhv, SnapThreadCallback, this);					
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bOpen = TRUE;		//标志已经打开Snap环境
	//启动数字摄像机采集图像到内存
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hhv, ppBuf,1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart = TRUE;
}

void CSMTDlg::OnCamera_Stop()
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
		m_bStart = FALSE;
}

void CSMTDlg::OnCamera_Close()
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
		m_bStart = FALSE;
	status = HVCloseSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
	{
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
		m_nError = 0;
		m_nLost  = 0;
	}
}

void CSMTDlg::OnCamera_SavePic()
{
	//	以下保存BMP文件设置基本相同
	Mat saveImg = m_src.clone();
	CString defaultDir = _T("E:\\image_avi_save");   //默认打开的文件路径
	CString filter = _T("Bitmap Files(*.bmp)|*.bmp");
	CFileDialog dlg(FALSE, defaultDir, _T("img") ,OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, filter, this);
	if (dlg.DoModal() == IDOK) 
	{
		CString imgPath = dlg.GetPathName();
		if (!imgPath.IsEmpty())
		{
			USES_CONVERSION;
			string imgPathAndName = T2CA(imgPath);
			imwrite(imgPathAndName, saveImg);
		}
		else
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}

void CSMTDlg::OnCamera_SaveVideo()
{
	CWinThread *pStoreVideoThread = NULL;
	pStoreVideoThread = AfxBeginThread(&StoreVideoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
	return;
}

UINT CSMTDlg::StoreVideoThreadFunc(LPVOID lpParam)
{
	CSMTDlg *pCSMTDlg = (CSMTDlg*)lpParam;
	pCSMTDlg->StoreVideo();
	return 0;
}

void CSMTDlg::StoreVideo()
{
	m_bIsCapture = TRUE;
	CString defaultDir = _T("E:\\image_avi_save");   //默认打开的文件路径
	CString strAVIFileName = _T("");
	CString filter = _T("AVI Files (*.avi)|*.avi;All Files (*.*)|*.*||");   //文件过虑的类型 
	CFileDialog dlg(FALSE, defaultDir, _T("Video"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if(dlg.DoModal() == IDOK)
	{
		strAVIFileName = dlg.GetPathName();
		if (!strAVIFileName.IsEmpty())
		{
			USES_CONVERSION;
			string videoPathAndName = T2CA(strAVIFileName);
			VideoWriter camera1_Writer(videoPathAndName, CV_FOURCC('M', 'J', 'P', 'G'), 60.0, Size(640, 512));
			while(m_bIsCapture)
			{
				Mat frame = m_src.clone();	
				camera1_Writer << frame;
				waitKey(27);
				if (m_bIsCapture == FALSE )
					break;
			}
		}
		else
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}

void CSMTDlg::OnCamera_StopVideo()
{
	m_bIsCapture = FALSE;
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

void CSMTDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	int ctrlID=pScrollBar->GetDlgCtrlID();
	int nPosition(0);
	switch(ctrlID)
	{
	case IDC_SLIDER_GAIN:
		nPosition=m_sliderGain.GetPos();
		m_spinGain.SetPos(nPosition);
		m_editGain = nPosition;
		SetGain(IDC_SLIDER_GAIN);
		break;
	case IDC_SPIN_GAIN:
		nPosition=m_spinGain.GetPos();
		m_sliderGain.SetPos(nPosition);
		m_editGain = nPosition;
		SetGain(IDC_SPIN_GAIN);
		break;
	case IDC_SLIDER_SHUTTER:
		nPosition=m_sliderShutter.GetPos();
		m_spinShutter.SetPos(nPosition);
		m_editShutter = nPosition;
		SetExposureTime(m_hhv, Width, nPosition, ExposureTint_Lower, m_lHBlanking, SnapSpeed, Resolution);
		break;
	case IDC_SPIN_SHUTTER:
		nPosition=m_spinShutter.GetPos();
		m_sliderShutter.SetPos(nPosition);
		m_editShutter = nPosition;
		SetExposureTime(m_hhv, Width, nPosition, ExposureTint_Lower, m_lHBlanking, SnapSpeed,Resolution);
		break;
	default:
		break;
	}
	UpdateData(FALSE);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSMTDlg::SetGain(int ctrID)
{
	HVSTATUS status=STATUS_OK;
	long nGain;
	switch(ctrID)
	{
	case IDC_SLIDER_GAIN:
		{
			nGain = m_sliderGain.GetPos();
			for (int nChannel=RED_CHANNEL; nChannel<=BLUE_CHANNEL; nChannel++)
			{
				status=HVAGCControl(m_hhv, nChannel, nGain);
				HV_VERIFY(status);
			}
		}
		break;
	case IDC_SPIN_GAIN:
		{
			nGain = m_spinGain.GetPos();
			for (int nChannel=RED_CHANNEL; nChannel<=BLUE_CHANNEL; nChannel++)
			{
				status=HVAGCControl(m_hhv, nChannel, nGain);
				HV_VERIFY(status);
			}
		}
		break;
	default:
		break;
	}	
	return TRUE;
}

BOOL CSMTDlg::InitDMC3000Card()
{
// 	WORD My_CardNum ;      //定义卡数
// 	WORD My_CardList[8];   //定义卡号数组
// 	DWORD My_CardTypeList[8];   //定义各卡类型
// 	if( dmc_board_init() <= 0 )      //控制卡的初始化操作
// 		MessageBox(_T("初始化控制卡失败！"), _T("出错"));
// 	dmc_get_CardInfList(&My_CardNum, My_CardTypeList, My_CardList);    //获取正在使用的卡号列表
// 	m_nCard = My_CardList[0]; 

	return TRUE;
}

void CSMTDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int curSel = m_tab.GetCurSel();
	switch(curSel)
	{
	case 0:
		m_manualDlg.ShowWindow(TRUE);
		m_semiAutoDlg.ShowWindow(FALSE);
		break;
	case 1:
		m_manualDlg.ShowWindow(FALSE);
		m_semiAutoDlg.ShowWindow(TRUE);
		break;
	default:
		break;
	}
	*pResult = 0;
}
