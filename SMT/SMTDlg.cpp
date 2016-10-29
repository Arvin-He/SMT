// SMTDlg.cpp : implementation file
#include "stdafx.h"
#include "SMT.h"
#include "SMTDlg.h"
#include "afxdialogex.h"
#include "ErrorBox.h"
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MAX
#define MAX(a,b)      (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)      (((a) < (b)) ? (a) : (b))
#endif

//�����Լ���0
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_src = Mat(Size(640, 512), CV_8UC3);
	InitialDHCamera();
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
	SetWindowText(_T("��Ƭ��ϵͳ"));
	//add menu
	if (!m_menu.LoadMenu(IDR_MENU))
	{
		TRACE0("�˵�����ʧ�ܣ�");
		return -1;
	}
	SetMenu(&m_menu);
	//add toolBar
	if (!m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(4,4,0,0))
		|| !m_toolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("����������ʧ�ܣ�");
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

//////////////////////////CCD����///////////////////////////
//��ȡbayer��ʽ
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

/*	����:SetExposureTime
	�������:int nWindWidth			��ǰͼ����		
			 int lTintUpper			�ع�ʱ��ķ��ӣ�shutter��ȡֵ
			 int lTintLower			�ع�ʱ��ķ�ĸ����shutter�ĵ�λ���	��ms:1000��s:1��
	�������:��						
	˵��:�����ع�ʱ�䣨�����Ĳ����������ʱ��Ƶ�ʣ�����ֵ��ȡĬ��ֵ��*/
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

/*	����:OnSnapError��
	�������:WPARAM wParam û��ʹ��	,LPARAM lParam �쳣״̬�룻
    �������:LRESULT��˵��:������ɼ��쳣���󱨸�*/
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
	//	ֹͣ�ɼ�ͼ���ڴ棬�����ٴε���HVStartSnapEx��������������ɼ�
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart = FALSE;
	return TRUE;
}

BOOL CSMTDlg::DestroyDHCamera(HHV hDHCamera, BYTE *pRawBuffer, BYTE *pImageBuffer)
{
	HVSTATUS status = STATUS_OK;
	//	�ر�������������ͷ�����������ڲ���Դ
	status = EndHVDevice(hDHCamera);
	HV_VERIFY(status);
	//	����ͼ�񻺳���
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

/*	����:SnapThreadCallback
	�������:SNAP_INFO *pInfo��SNAP_INFO�ṹ������ǰ���������SNAPִ��״̬���������:int						
	˵��:����������ɼ����ڴ�ص����������û�һ�㲻�õ��ã����û��ṩ��SDKʹ�ã�
	�û��ڻص�������ʵ�ֶԲɼ����ݵĴ������ʾ����*/
int CALLBACK CSMTDlg::SnapThreadCallback(HV_SNAP_INFO *pInfo)
{
	CSMTDlg *This = (CSMTDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	HVSTATUS status = This->GetLastStatus();	// ��ȡ�����ͼ��ɼ�����״̬	
	if(!HV_SUCCESS(status)) //����������ò��ɹ��������ֹͣͼ��ɼ���
	{	
		::PostMessage(hwnd, WM_SNAP_STOP, 0, 0);
		return 1;
	}
	TRACE("LastStatus is %X", This->dwLastStatus);
	if(This->dwLastStatus == 0)	// ��״ֵ̬����0ʱ����ʾ�����ͼ��ɼ������������SendMessage��ʾͼ��
	{
	   /*�����Զ�����ϢWM_SNAP_EX_CHANGE�������ڣ�ͬʱ���뵱ǰ���Դ����ͼ�����
		*ע�⣺��SendMessage������Ϣ������ȴ���Ϣ������Ϻ󣬲����˳�����SendMessage����*/
		::PostMessage(hwnd, WM_SNAP_CHANGE, 0, 0);
		return 1;	
	}
	else if(This->dwLastStatus == STATUS_FILE_INVALID)// ��״ֵ̬������-18ʱ����ʾ���������1�ζ�֡��Ϊ,���������ٲɼ�ʱ��1�ζ�֡��Ϊ���ܰ���1֡����ͼ��
	{
		This->m_nLost++;	//��֡����
	}
	else if (This->dwLastStatus == 0xc0000012 || This->dwLastStatus == 0xc0000011)
	{
		This->m_nError++;	//�������
	}
	else // ��״ֵ̬Ϊ������0ֵʱ����ʾ�����ͼ��ɼ��쳣
	{
	   //�����Զ�����Ϣ WM_SNAP_ERROR �������ڣ�ע�⣺��PostMessage������Ϣ�����صȴ���Ϣ�����꣬�Ϳ��Է��ء�
		::PostMessage(hwnd, WM_SNAP_ERROR, 0, This->dwLastStatus);
	}	
	return 1;
}

/*	����:OnSnapChange1
	�������:WPARAM wParam	�ֲ���������Ϣ��Ϊ��ǰ���Դ����ͼ�����; LPARAM lParam	û��ʹ�ã�
	�������:LRESULT��˵��:ʵ�ֶԲɼ����ݵĴ������ʾ*/
LRESULT CSMTDlg::OnSnapChange(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status = STATUS_OK;	
	//	��ԭʼͼ�����ݽ���Bayerת����ת����Ϊ24λ��ͬʱ��ԭʼ���ݽ������·�ת
	ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer, Width, Height, ConvertType, m_pLutR, m_pLutG, m_pLutB, false, m_Layout);	
	m_src.data = (uchar*)m_pImageBuffer;
//	flip(m_src1, m_src1, -1);
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

/*	��ʼ�����������Ӳ��״̬���û�Ҳ����������λ�ó�ʼ���������������Ӧ��֤����������Ѿ��򿪣�
	�����û���Ӧ�ó����ʼ��ʱ��ͬʱ��ʼ�����������Ӳ���� */
BOOL CSMTDlg::InitialDHCamera()
{
	//	��ʼ�����г�Ա������ͬʱ�����������
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
	//	����������� 1
	status = BeginHVDevice(1, &m_hhv);
	//	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��
	HV_VERIFY(status);
	m_Layout = HVGetBayerType(m_hhv);
	//	��������������ֱ���
	HVSetResolution(m_hhv, Resolution);		
	//	�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)
	HVSetSnapMode(m_hhv, SnapMode);	
	//  ���ø�������������
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
	//  ����ADC�ļ���
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);
	//  ��ȡ�豸����
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv, DESC_DEVICE_TYPE, &type, &size);	
	//����������
	HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
	/*��Ƶ������ڣ�����Ƶ�����Χ���������ȡֵ��Χ���������봰�ڷ�Χ���ڣ���Ƶ�������Ͻ�X����ʹ��ڿ��ӦΪ4�ı�����
	���Ͻ�Y����ʹ��ڸ߶�ӦΪ2�ı�����������ڵ���ʼλ��һ������Ϊ(0, 0)���ɡ�*/
	HVSetOutputWindow(m_hhv, XStart, YStart, Width, Height);	
	//���òɼ��ٶ�
	HVSetSnapSpeed(m_hhv, SnapSpeed);
	//�����ع�ʱ��
	SetExposureTime(m_hhv, Width, ExposureTint_Upper, ExposureTint_Lower, m_lHBlanking, SnapSpeed, Resolution);	
	//	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
	m_pBmpInfo					= (BITMAPINFO *)m_chBmpBuf;
	//	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
	m_pBmpInfo->bmiHeader.biSize	= sizeof(BITMAPINFOHEADER);
	//	ͼ���ȣ�һ��Ϊ������ڿ��
	m_pBmpInfo->bmiHeader.biWidth	= Width;
	//	ͼ���ȣ�һ��Ϊ������ڸ߶�
	m_pBmpInfo->bmiHeader.biHeight	= Height;	
	/*	��������һ����ͬ�����ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��*/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression	    = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed		    = 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;	
	/*	����ԭʼͼ�񻺳�����һ�������洢�ɼ�ͼ��ԭʼ����
	*  һ��ͼ�񻺳�����С��������ڴ�С����Ƶ��ʽȷ����*/
	m_pRawBuffer = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer);
	//����Bayerת����ͼ�����ݻ���
	m_pImageBuffer = new BYTE[Width * Height * 3];
	ASSERT(m_pImageBuffer);
	return TRUE;
}
////////////////////////////CCD�ĺ���//////////////////////////////////////////////

void CSMTDlg::OnCamera_Open()
{
	HVSTATUS status = STATUS_OK;
	//��ʼ������������ɼ�ͼ���ڴ�Ŀ��ƣ�ָ���ص�����SnapThreadCallback��thisָ��
	status = HVOpenSnap(m_hhv, SnapThreadCallback, this);					
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bOpen = TRUE;		//��־�Ѿ���Snap����
	//��������������ɼ�ͼ���ڴ�
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
	//	ֹͣ�ɼ�ͼ���ڴ棬�����ٴε���HVStartSnapEx��������������ɼ�
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
		m_bStart = FALSE;
}

void CSMTDlg::OnCamera_Close()
{
	HVSTATUS status =STATUS_OK;
	//	ֹͣ�ɼ�ͼ���ڴ棬�����ٴε���HVStartSnapEx��������������ɼ�
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
	//	���±���BMP�ļ����û�����ͬ
	Mat saveImg = m_src.clone();
	CString defaultDir = _T("E:\\image_avi_save");   //Ĭ�ϴ򿪵��ļ�·��
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
			MessageBox(_T("�ļ�·��Ϊ�գ�\n"));
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
	CString defaultDir = _T("E:\\image_avi_save");   //Ĭ�ϴ򿪵��ļ�·��
	CString strAVIFileName = _T("");
	CString filter = _T("AVI Files (*.avi)|*.avi;All Files (*.*)|*.*||");   //�ļ����ǵ����� 
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
			MessageBox(_T("�ļ�·��Ϊ�գ�\n"));
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