// SMTDlg.cpp : implementation file
#include "stdafx.h"
#include "SMT.h"
#include "SMTDlg.h"
#include "afxdialogex.h"
#include "ErrorBox.h"
#include "CvvImage.h"
#include "dmc3000/inc/LTDMC.h"
#include "global.h"

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
const HV_RESOLUTION				Resolution			= RES_MODE0;
const HV_SNAP_MODE				SnapMode			= CONTINUATION;
const HV_BAYER_CONVERT_TYPE		ConvertType			= BAYER2RGB_NEIGHBOUR1;
//const long						Gain				= 10;
const long						ExposureTint_Upper	= 60;
const long						ExposureTint_Lower	= 1000;
const long						ADCLevel			= ADC_LEVEL2;
const int						XStart              = 0;
const int						YStart              = 0;
const int						Width               = 640;
const int						Height              = 480;
//const HV_SNAP_SPEED				SnapSpeed			= HIGH_SPEED;
const HV_SNAP_SPEED				SnapSpeed			= NORMAL_SPEED;
// CSMTDlg dialog
CSMTDlg::CSMTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMTDlg::IDD, pParent)
	, m_editShutter(600)
	, m_editGain(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_src = Mat(Size(640, 480), CV_8UC3);
	
	m_bIsCapture = FALSE;
	
}

CSMTDlg::~CSMTDlg()
{
	dmc_board_close();	//非常之重要，释放其占用的系统资源
	OnCamera_Close();   
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
	DDX_Control(pDX, IDC_TAB2, m_bottomTab);
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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CSMTDlg::OnTcnSelchangeTab2)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_SET_STAGE_X_HOME_BTN, &CSMTDlg::OnClickedSetStageXHomeBtn)
	ON_BN_CLICKED(IDC_SET_STAGE_Y_HOME_BTN, &CSMTDlg::OnClickedSetStageYHomeBtn)
	ON_BN_CLICKED(IDC_SET_CCD_X_HOME_BTN, &CSMTDlg::OnClickedSetCcdXHomeBtn)
	ON_BN_CLICKED(IDC_SET_CCD_Z_HOME_BTN, &CSMTDlg::OnClickedSetCcdZHomeBtn)
	ON_BN_CLICKED(IDC_STAGE_X_GOHOME_BTN, &CSMTDlg::OnClickedStageXGohomeBtn)
	ON_BN_CLICKED(IDC_STAGE_Y_GOHOME_BTN, &CSMTDlg::OnClickedStageYGohomeBtn)
	ON_BN_CLICKED(IDC_CCD_X_GOHOME_BTN, &CSMTDlg::OnClickedCcdXGohomeBtn)
	ON_BN_CLICKED(IDC_CCD_Z_GOHOME_BTN, &CSMTDlg::OnClickedCcdZGohomeBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STAGE_X_STOP_BTN, &CSMTDlg::OnClickedStageXStopBtn)
	ON_BN_CLICKED(IDC_STAGE_Y_STOP_BTN, &CSMTDlg::OnClickedStageYStopBtn)
	ON_BN_CLICKED(IDC_CCD_X_STOP_BTN, &CSMTDlg::OnClickedCcdXStopBtn)
	ON_BN_CLICKED(IDC_CCD_Z_STOP_BTN, &CSMTDlg::OnClickedCcdZStopBtn)
END_MESSAGE_MAP()


// CSMTDlg message handlers

BOOL CSMTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
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
	m_sliderGain.SetTicFreq(2);

	m_spinShutter.SetRange(0, 1000);
	m_spinShutter.SetPos(m_editShutter);
	m_spinShutter.SetBuddy(GetDlgItem(IDC_EDIT_SHUTTER));
	m_sliderShutter.SetRange(0, 1000);
	m_sliderShutter.SetPos(m_editShutter);
	m_sliderShutter.SetTicFreq(25);

	GetDlgItem(IDC_SHOW_PIC)->MoveWindow(10, 80, Width, Height, true); 

	m_tab.InsertItem(0, _T("手动控制"));
	m_tab.InsertItem(1, _T("自动控制"));

	m_manualDlg.Create(IDD_MANUAL, GetDlgItem(IDC_TAB));
	m_semiAutoDlg.Create(IDD_SEMI_AUTO, GetDlgItem(IDC_TAB));

	m_manualDlg.EnableWindow(TRUE);
	m_semiAutoDlg.EnableWindow(TRUE);

	CRect mainDlgRect(0, 0, 0, 0);
	GetClientRect(&mainDlgRect);
	mainDlgRect.top += 260;
	mainDlgRect.left += 680;
	mainDlgRect.bottom -= 10;
	mainDlgRect.right -= 10;
	m_tab.MoveWindow(&mainDlgRect);

	CRect tabRect(0,0,0,0);
	m_tab.GetClientRect(&tabRect);
	tabRect.top += 20;
	tabRect.left += 10;
	tabRect.bottom -= 10;
	tabRect.right -= 10;
	m_manualDlg.MoveWindow(&tabRect);
	m_semiAutoDlg.MoveWindow(&tabRect);

	m_manualDlg.ShowWindow(SW_SHOW);
	m_semiAutoDlg.ShowWindow(SW_HIDE);
	m_tab.SetCurSel(0);


	m_bottomTab.InsertItem(0, _T("图像辅助"));
	m_bottomTab.InsertItem(1, _T("设置"));
	m_bottomTab.InsertItem(3, _T("运动参数设置"));
	m_imageAssistDlg.Create(IDD_IMAGE_ASSIST_DIALOG, GetDlgItem(IDC_TAB2));
	m_settingDlg.Create(IDD_SETTING_DIALOG, GetDlgItem(IDC_TAB2));
	m_setParamDlg.Create(IDD_SETTING_SOFTWARE_DIALOG, GetDlgItem(IDC_TAB2));
	m_imageAssistDlg.EnableWindow(TRUE);
	m_settingDlg.EnableWindow(TRUE);
	m_setParamDlg.EnableWindow(TRUE);
	CRect bottomTabRect(0, 0, 0, 0);
	m_bottomTab.GetClientRect(&bottomTabRect);
	bottomTabRect.top += 20;
	bottomTabRect.left += 10;
	bottomTabRect.bottom -= 10;
	bottomTabRect.right -= 10;
	m_imageAssistDlg.MoveWindow(&bottomTabRect);
	m_settingDlg.MoveWindow(&bottomTabRect);
	m_setParamDlg.MoveWindow(&bottomTabRect);
	m_imageAssistDlg.ShowWindow(SW_SHOW);
	m_settingDlg.ShowWindow(SW_HIDE);
	m_setParamDlg.ShowWindow(SW_HIDE);
	m_bottomTab.SetCurSel(0);
	m_imageAssistDlg.m_pSMTDlg = this;
	// 初始化相机
	InitialDHCamera();
	InitDMC3000Card(); // 初始化DMC3000运动控制卡
	InitDMC3000Status();
	SetTimer(0, 500, NULL);
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
	if(STATUS_OK != status)
	{
		Layout = BAYER_GR;
	}
	else
	{
		BYTE *pbBayerLayout = NULL;
		pbBayerLayout = new BYTE[nSize];
		status = HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, pbBayerLayout,&nSize);
		Layout = (HV_BAYER_LAYOUT)*pbBayerLayout;
		delete []pbBayerLayout; 
		pbBayerLayout=NULL;
	}
	return Layout;
}

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
	lClockFreq = (SnapSpeed == HIGH_SPEED) ? 24000000 : 12000000;
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
	if (ErrDlg.m_dwStatus == dwLastStatus)
	{
		if (ErrDlg.DoModal() == IDOK)
		{
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

/*	函数:OnSnapChange
	输入参数:WPARAM wParam	字参数，在消息中为当前可以处理的图像序号; LPARAM lParam	没有使用；
	输出参数:LRESULT；说明:实现对采集数据的处理和显示*/
LRESULT CSMTDlg::OnSnapChange(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status = STATUS_OK;	
	//	将原始图像数据进行Bayer转换，转换后为24位。同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer, Width, Height, ConvertType, m_pLutR, m_pLutG, m_pLutB, false, m_Layout);	
	g_src.data = (uchar*)m_pImageBuffer;
	//m_src.data = (uchar*)m_pImageBuffer;
	//flip(g_src, g_src, -1);
	if (m_imageAssistDlg.m_bDrawCross)
		DrawCross(g_src);
	if (m_imageAssistDlg.m_bDrawScale)
		DrawImgScale(g_src);
	if (m_imageAssistDlg.m_bDrawLine)
		DrawLine(g_src);
	if (m_imageAssistDlg.m_bDrawRect)
		DrawRect(g_src);
	if (m_imageAssistDlg.m_bDrawCircle)
		DrawCircle(g_src);
	if (m_imageAssistDlg.m_bMeasureDis)
		MeasureDis(g_src);

	ShowImage(g_src, IDC_SHOW_PIC);	
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
		//HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
		HVAGCControl(m_hhv, RED_CHANNEL + i, m_editGain);
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
	SetExposureTime(m_hhv, Width, m_editShutter, ExposureTint_Lower, m_lHBlanking, SnapSpeed, Resolution);	
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
	//Mat saveImg = m_src.clone();
	Mat saveImg = g_src.clone();
	CString defaultDir = _T("D:\\image_avi_save");   //默认打开的文件路径
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
	CString defaultDir = _T("D:\\image_avi_save");   //默认打开的文件路径
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
			VideoWriter camera1_Writer(videoPathAndName, 
				CV_FOURCC('M', 'J', 'P', 'G'), 30.0, Size(Width, Height));
			while(m_bIsCapture)
			{
				//Mat frame = m_src.clone();	
				Mat frame = g_src.clone();	
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

void CSMTDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int curSel = m_tab.GetCurSel();
	switch(curSel)
	{
	case 0:
		{
			m_manualDlg.ShowWindow(TRUE);
			m_semiAutoDlg.ShowWindow(FALSE);
		}
		break;
	case 1:
		{
			m_manualDlg.ShowWindow(FALSE);
			m_semiAutoDlg.ShowWindow(TRUE);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CSMTDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int curSel = m_bottomTab.GetCurSel();
	switch(curSel)
	{
	case 0:
		{
			m_imageAssistDlg.ShowWindow(TRUE);
			m_settingDlg.ShowWindow(FALSE);
			m_setParamDlg.ShowWindow(FALSE);
		}
		break;
	case 1:
		{
			m_imageAssistDlg.ShowWindow(FALSE);
			m_settingDlg.ShowWindow(TRUE);
			m_setParamDlg.ShowWindow(FALSE);
		}
		break;
	case 2:
		{
			m_imageAssistDlg.ShowWindow(FALSE);
			m_settingDlg.ShowWindow(FALSE);
			m_setParamDlg.ShowWindow(TRUE);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

BOOL CSMTDlg::InitDMC3000Card()
{
	WORD My_CardNum ;					//定义卡数
	WORD My_CardList[8];				//定义卡号数组
	DWORD My_CardTypeList[8];			//定义各卡类型
	if( dmc_board_init() <= 0 )			//控制卡的初始化操作
		MessageBox(_T("初始化控制卡失败！"), _T("出错"));
	dmc_get_CardInfList(&My_CardNum, My_CardTypeList, My_CardList);    //获取正在使用的卡号列表
	//m_nCard = My_CardList[0];
	g_nCardNo = My_CardList[0];

	for (int i=0; i<g_nAxisCount; i++)
	{
		// 设定脉冲模式及逻辑方向	
		dmc_set_pulse_outmode(g_nCardNo, i, 4);
		// 设置硬限位
		dmc_set_el_mode(g_nCardNo, i, 1, 1, 0);
		// 设置软限位
		//dmc_set_softlimit(g_nCardNo, i, 1, 1, 0, 0, )
		// 编码器设置
		dmc_set_counter_inmode(g_nCardNo, i, 0);
		//设置EZ
		dmc_set_ez_mode(g_nCardNo, i, 0);
		// 设置回原点信号
		// dmc_set_home_pin_logic(g_nCardNo, i, 1);
		// 设置回原点模式,和方向有关
		//dmc_set_homemode(g_nCardNo, i, 0, 0, 0);
	}
	return TRUE;
}

BOOL CSMTDlg::ResetDMC3000Card()
{
	short status = dmc_board_reset();
	// 板卡重置需要5s后才能初始化
	Sleep(6000);
	return TRUE;
}

// 画十字
void CSMTDlg::DrawCross(Mat img)
{
	Point centerPt(Width/2, Height/2);
	circle(img, centerPt, 15, Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x-20, centerPt.y), 
		Point(centerPt.x+20, centerPt.y), Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x, centerPt.y-20), 
		Point(centerPt.x, centerPt.y+20), Scalar(0,255,0), 1, CV_AA);
}

// 画比例尺
void CSMTDlg::DrawImgScale(Mat img)
{
	rectangle(img, Point(568, 458), Point(630, 461), Scalar(0,255,0), -1, CV_AA);
	putText(img, "??um", Point(574, 453), FONT_HERSHEY_COMPLEX_SMALL, 0.6, Scalar(0,255,0), 1, CV_AA);
}

void CSMTDlg::DrawLine(Mat img)
{
	if (m_imageAssistDlg.m_bDrawLine && m_drawLinePoints.size() == 2)
		line(img, m_drawLinePoints[0], m_drawLinePoints[1], Scalar(255, 255, 0), 2, CV_AA);
}

 void CSMTDlg::DrawRect(Mat img)
 {
 	if (m_imageAssistDlg.m_bDrawRect && m_drawRectPoints.size() == 3)
 	{
 		line(img, m_drawRectPoints[0], m_drawRectPoints[1], Scalar(255, 255, 0), 2, CV_AA);
 		line(img, m_drawRectPoints[1], m_drawRectPoints[2], Scalar(255, 255, 0), 2, CV_AA);
 		line(img, m_drawRectPoints[2], Point(m_drawRectPoints[0].x+(m_drawRectPoints[2].x-m_drawRectPoints[1].x), 
 			m_drawRectPoints[2].y+(m_drawRectPoints[0].y-m_drawRectPoints[1].y)), Scalar(255, 255, 0), 2, CV_AA);
 		line(img, Point(m_drawRectPoints[0].x+(m_drawRectPoints[2].x-m_drawRectPoints[1].x), 
 			m_drawRectPoints[2].y+(m_drawRectPoints[0].y-m_drawRectPoints[1].y)), m_drawRectPoints[0], Scalar(255, 255, 0), 2, CV_AA);
 	}
 }
 
 void CSMTDlg::DrawCircle(Mat img)
 {
 	if (m_imageAssistDlg.m_bDrawCircle && m_drawCirclePoints.size() == 2)
 	{
 		int disX = abs(m_drawCirclePoints[1].x-m_drawCirclePoints[0].x);
 		int disY = abs(m_drawCirclePoints[1].y-m_drawCirclePoints[0].y);
 		double radius = sqrt((double)(disX*disX) + (double)(disY*disY));
 		circle(img, m_drawCirclePoints[0], (int)radius, Scalar(0, 255, 255), 2, CV_AA);
 	}
 }

void CSMTDlg::MeasureDis(Mat img)
{
	if (m_imageAssistDlg.m_bMeasureDis && m_measureDisPoints.size() == 2)
	{
		int disX = abs(m_measureDisPoints[1].x - m_measureDisPoints[0].x);
		int disY = abs(m_measureDisPoints[1].y - m_measureDisPoints[0].y);
		double dis = sqrt((double)(disX*disX) + (double)(disY*disY));
		line(img, m_measureDisPoints[0], m_measureDisPoints[1], Scalar(0, 255, 0), 2, CV_AA);
		CString strDis(_T(""));
		strDis.Format("%.3f", dis);
		strDis = strDis+_T("pixs");
		int flagX(1), flagY(1);
		if (m_measureDisPoints[1].x > m_measureDisPoints[0].x)
			flagX = 1;
		else
			flagX = -1;
		if (m_measureDisPoints[1].y > m_measureDisPoints[0].y)
			flagY = 1;
		else
			flagY = -1;
		putText(img, (string)strDis, Point(m_measureDisPoints[0].x+(disX/2)*flagX, 
				m_measureDisPoints[0].y+(disY/2)*flagY),
				FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 255), 1, CV_AA);
	}
}

void CSMTDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CvPoint pt;
	pt.x = point.x - 10;
	pt.y = point.y - 80;
	if (m_imageAssistDlg.m_bDrawLine && m_drawLinePoints.size() < 3)
	{
		m_drawLinePoints.push_back(pt);
	}
	if (m_imageAssistDlg.m_bDrawRect && m_drawRectPoints.size() < 4)
	{
		m_drawRectPoints.push_back(pt);
	}
	if (m_imageAssistDlg.m_bDrawCircle && m_drawCirclePoints.size() < 3)
	{
		m_drawCirclePoints.push_back(pt);
	}
	if (m_imageAssistDlg.m_bMeasureDis && m_measureDisPoints.size() < 3)
	{
		m_measureDisPoints.push_back(pt);
	}
	if (m_imageAssistDlg.m_bMeasureAngle && m_measureAnglePoints.size() < 5)
	{
		m_measureAnglePoints.push_back(pt);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSMTDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CSMTDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CSMTDlg::OnClickedSetStageXHomeBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_position(g_nCardNo, 0, 0);
}


void CSMTDlg::OnClickedSetStageYHomeBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_position(g_nCardNo, 1, 0);
}


void CSMTDlg::OnClickedSetCcdXHomeBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_position(g_nCardNo, 2, 0);
}


void CSMTDlg::OnClickedSetCcdZHomeBtn()
{
	// TODO: Add your control notification handler code here
	dmc_set_position(g_nCardNo, 3, 0);
}


void CSMTDlg::OnClickedStageXGohomeBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);//刷新参数
	dmc_set_pulse_outmode(g_nCardNo, 0, 0);  //设置脉冲输出模式
	dmc_set_profile(g_nCardNo, 0, 100, 1000, 0.02, 0.02, 500);//设置速度曲线
	dmc_set_homemode(g_nCardNo, 0, 0, 1, 0, 1);//设置回零方式
	dmc_home_move(g_nCardNo, 0);//回零动作
	while (dmc_check_done(g_nCardNo, 0) == 0)      //判断当前轴状态 0：指定轴正在运行，1：指定轴已停止
	{
		AfxGetApp()->PumpMessage();
		GetDlgItem(IDC_STAGE_X_GOHOME_BTN)->EnableWindow(false); 
	}
	GetDlgItem(IDC_STAGE_X_GOHOME_BTN)->EnableWindow(true); 
	UpdateData(false);
}


void CSMTDlg::OnClickedStageYGohomeBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);//刷新参数
	dmc_set_pulse_outmode(g_nCardNo, 1, 0);  //设置脉冲输出模式
	dmc_set_profile(g_nCardNo, 1, 100, 1000, 0.02, 0.02, 500);//设置速度曲线
	dmc_set_homemode(g_nCardNo, 1, 0, 1, 0, 1);//设置回零方式
	dmc_home_move(g_nCardNo, 1);//回零动作
	while (dmc_check_done(g_nCardNo, 1) == 0)      //判断当前轴状态 0：指定轴正在运行，1：指定轴已停止
	{
		AfxGetApp()->PumpMessage();
		GetDlgItem(IDC_STAGE_Y_GOHOME_BTN)->EnableWindow(false); 
	}
	GetDlgItem(IDC_STAGE_Y_GOHOME_BTN)->EnableWindow(true); 
	UpdateData(false);
}


void CSMTDlg::OnClickedCcdXGohomeBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);//刷新参数
	dmc_set_pulse_outmode(g_nCardNo, 2, 0);  //设置脉冲输出模式
	dmc_set_profile(g_nCardNo, 2, 100, 1000, 0.02, 0.02, 500);//设置速度曲线
	dmc_set_homemode(g_nCardNo, 2, 0, 1, 0, 1);//设置回零方式
	dmc_home_move(g_nCardNo, 2);//回零动作
	while (dmc_check_done(g_nCardNo, 2) == 0)      //判断当前轴状态 0：指定轴正在运行，1：指定轴已停止
	{
		AfxGetApp()->PumpMessage();
		GetDlgItem(IDC_CCD_X_GOHOME_BTN)->EnableWindow(false); 
	}
	GetDlgItem(IDC_CCD_X_GOHOME_BTN)->EnableWindow(true); 
	UpdateData(false);
}


void CSMTDlg::OnClickedCcdZGohomeBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);//刷新参数
	dmc_set_pulse_outmode(g_nCardNo, 3, 0);  //设置脉冲输出模式
	dmc_set_profile(g_nCardNo, 3, 100, 1000, 0.02, 0.02, 500);//设置速度曲线
	dmc_set_homemode(g_nCardNo, 3, 0, 1, 0, 1);//设置回零方式
	dmc_home_move(g_nCardNo, 3);//回零动作
	while (dmc_check_done(g_nCardNo, 3) == 0)      //判断当前轴状态 0：指定轴正在运行，1：指定轴已停止
	{
		AfxGetApp()->PumpMessage();
		GetDlgItem(IDC_CCD_Z_GOHOME_BTN)->EnableWindow(false); 
	}
	GetDlgItem(IDC_CCD_Z_GOHOME_BTN)->EnableWindow(true); 
	UpdateData(false);
}

void CSMTDlg::InitDMC3000Status()
{
	SetDMC3000Status(FALSE, IDC_STAGE_X_EL_UP);
	SetDMC3000Status(FALSE, IDC_STAGE_X_EL_DOWN);
	SetDMC3000Status(FALSE, IDC_STAGE_X_ORG);
	SetDMC3000Status(FALSE, IDC_STAGE_Y_EL_UP);
	SetDMC3000Status(FALSE, IDC_STAGE_Y_EL_DOWN);
	SetDMC3000Status(FALSE, IDC_STAGE_Y_ORG);
	SetDMC3000Status(FALSE, IDC_CCD_X_EL_UP);
	SetDMC3000Status(FALSE, IDC_CCD_X_EL_DOWN);
	SetDMC3000Status(FALSE, IDC_CCD_X_ORG);
	SetDMC3000Status(FALSE, IDC_CCD_Z_EL_UP);
	SetDMC3000Status(FALSE, IDC_CCD_Z_EL_DOWN);
	SetDMC3000Status(FALSE, IDC_CCD_Z_ORG);
}

// 设置DMC3000运动状态中一些io的图标
void CSMTDlg::SetDMC3000Status(BOOL status, int nID)
{
	CStatic *pStatus =(CStatic*)GetDlgItem(nID);
	pStatus->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	if (status)
	{
		pStatus->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_RED)));
	}
	else
	{
		pStatus->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_GREEN)));
	}
}

void CSMTDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 0:
		{
			UpdateDMC3000Data();
		}
	default:
		break;
	}
}

// 更新DMC3000运动状态
void CSMTDlg::UpdateDMC3000Data()
{
	UpdateDMC3000PulseAndDistance(0, IDC_STAGE_X_PULSE_EDIT, IDC_STAGE_X_POS_EDIT);
	UpdateDMC3000PulseAndDistance(1, IDC_STAGE_Y_PULSE_EDIT, IDC_STAGE_Y_POS_EDIT);
	UpdateDMC3000PulseAndDistance(2, IDC_CCD_X_PULSE_EDIT, IDC_CCD_X_POS_EDIT);
	UpdateDMC3000PulseAndDistance(3, IDC_CCD_Z_PULSE_EDIT, IDC_CCD_Z_POS_EDIT);
	UpdateDMC3000Status(0, IDC_STAGE_X_EL_UP, IDC_STAGE_X_EL_DOWN, IDC_STAGE_X_ORG);
	UpdateDMC3000Status(1, IDC_STAGE_Y_EL_UP, IDC_STAGE_Y_EL_DOWN, IDC_STAGE_Y_ORG);
	UpdateDMC3000Status(2, IDC_CCD_X_EL_UP, IDC_CCD_X_EL_DOWN, IDC_CCD_X_ORG);
	UpdateDMC3000Status(3, IDC_CCD_Z_EL_UP, IDC_CCD_Z_EL_DOWN, IDC_CCD_Z_ORG);
}

// 更新状态位
void CSMTDlg::UpdateDMC3000Status(int nAxisIndex, int elupID, int eldownID, int orgID)
{
	int elup = dmc_axis_io_status(g_nCardNo, nAxisIndex) & 0x01;
	int eldown = dmc_axis_io_status(g_nCardNo, nAxisIndex) & 0x02;
	int org = dmc_axis_io_status(g_nCardNo, nAxisIndex) & 0x10;
	SetDMC3000Status(elup, elupID);
	SetDMC3000Status(eldown, eldownID);
	SetDMC3000Status(org, orgID);
}

// 更新脉冲数和距离
void CSMTDlg::UpdateDMC3000PulseAndDistance(int nAxisIndex, int nPulseID, int nDisID)
{
	long currentPulse = dmc_get_position(g_nCardNo, nAxisIndex); //获取当前轴位置
	double currentPos = TransPulseToDistance(nAxisIndex, currentPulse);
	m_strPulseCount.Format(_T("%ld"), currentPulse);
	m_strDistance.Format(_T("%ld"), currentPos);
	GetDlgItem(nPulseID)->SetWindowText(m_strPulseCount);
	GetDlgItem(nDisID)->SetWindowText(m_strDistance);
}

void CSMTDlg::OnClickedStageXStopBtn()
{
	// TODO: Add your control notification handler code here
	dmc_stop(g_nCardNo, 0, 0); //减速停止
}


void CSMTDlg::OnClickedStageYStopBtn()
{
	// TODO: Add your control notification handler code here
	dmc_stop(g_nCardNo, 1, 0); //减速停止
}


void CSMTDlg::OnClickedCcdXStopBtn()
{
	// TODO: Add your control notification handler code here
	dmc_stop(g_nCardNo, 2, 0); //减速停止
}


void CSMTDlg::OnClickedCcdZStopBtn()
{
	// TODO: Add your control notification handler code here
	dmc_stop(g_nCardNo, 3, 0); //减速停止
}
