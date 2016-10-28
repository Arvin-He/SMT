// MyHVControl.cpp: implementation of the MyHVControl class.
#include "stdafx.h"
#include "MyHVControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MyHVControl::MyHVControl()
{
	HVDeviceNum = 0;
	Resolution = RES_MODE2;
	SnapMode = CONTINUATION;
	SnapSpeed = HIGH_SPEED;
	Layout = BAYER_GR;
	ConvertType = BAYER2RGB_NEIGHBOUR;
	Gain = 8;
	ExposureTint_Upper = 50;
	ExposureTint_Lower = 1000;

	ShutterDelay = 0;
	ADCLevel = ADC_LEVEL2;

	ShowStartX = 0;
	ShowStartY = 0;
	ShowEndX = 800;
	ShowEndY = 640;
	ShowWidth = ShowEndX - ShowStartX;
	ShowHeight = ShowEndY - ShowStartY;

	Flag_HVControl = FALSE;
	Flag_Work = FALSE;
}

MyHVControl::~MyHVControl()
{
	if(HVDeviceNum >= 0) {HVDestory();}
}

BOOL MyHVControl::HVInitial(int SubHVNum,int SubShowWidth,int SubShowHeight)
{
	int SubMaxWidth = 2048;
	int SubMaxHeight = 1536;
	switch(Resolution)
	{
	case RES_MODE0:
		SubMaxWidth = 2048;
		SubMaxHeight = 1536;
		break;
	case RES_MODE1:
		SubMaxWidth = 1024;
		SubMaxHeight = 768;
		break;
	case RES_MODE2:
		SubMaxWidth = 640;
		SubMaxHeight = 480;
		break;
	case RES_MODE3:
		SubMaxWidth = 512;
		SubMaxHeight = 384;
		break;
	default:
		break;
	}
	return HVInitial(SubHVNum, (SubMaxWidth-SubShowWidth)/2, (SubMaxHeight-SubShowHeight)/2, SubShowWidth, SubShowHeight);
}

BOOL MyHVControl::HVInitial(int SubHVNum, int SubShowStartX, int SubShowStartY, int SubShowWidth, int SubShowHeight)
{
	status = STATUS_OK;
	m_bOpen			= FALSE;
	m_bStart		= FALSE;
	m_bNegative		= FALSE;
	m_pBmpInfo		= NULL;
	m_pRawBuffer	= NULL;
	m_pImageBuffer	= NULL;
	for(int i=0; i<256; i++)
	{
		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;
	}

	m_bIsToGetBadPixel = FALSE;
	m_bIsToGetTemplate = FALSE;
	m_bIsToRemoveBadPixel = FALSE;
	m_bIsUseTemplate = FALSE;
	m_nTempTimes = 0;

	int SubMaxWidth = 2048;
	int SubMaxHeight = 1536;
	switch(Resolution)
	{
	case RES_MODE0:
		SubMaxWidth = 2048;
		SubMaxHeight = 1536;
		break;
	case RES_MODE1:
		SubMaxWidth = 1024;
		SubMaxHeight = 768;
		break;
	case RES_MODE2:
		SubMaxWidth = 640;
		SubMaxHeight = 480;
		break;
	case RES_MODE3:
		SubMaxWidth = 512;
		SubMaxHeight = 384;
		break;
	default:
		break;
	}

	if(SubShowStartX < 0)
		SubShowStartX = 0;
	if(SubShowStartX >= SubMaxWidth)
		SubShowStartX = SubMaxWidth-1;

	if(SubShowStartY < 0)
		SubShowStartY = 0;
	if(SubShowStartY >= SubMaxHeight)
		SubShowStartY = SubMaxHeight-1;

	if(SubShowWidth < 0)
		SubShowWidth = 0;
	if(SubShowWidth > SubMaxWidth-SubShowStartX)
		SubShowWidth = SubMaxWidth-SubShowStartX;

	if(SubShowHeight < 0)
		SubShowHeight = 0;
	if(SubShowHeight > SubMaxHeight-SubShowStartY)
		SubShowHeight = SubMaxHeight-SubShowStartY;

	//	打开数字摄像机
	HVDeviceNum = SubHVNum;
	status = BeginHVDevice(HVDeviceNum, &m_hhv);
	//	检验函数执行状态，如果失败，则返回错误状态消息框
	if(status != STATUS_OK) {
		Flag_HVControl = FALSE;
		return FALSE;
	} else {
		Flag_HVControl = TRUE;
	}

	ShowStartX = SubShowStartX;
	ShowStartY = SubShowStartY;
	ShowEndX = SubShowStartX + SubShowWidth-1;
	ShowEndY = SubShowStartY + SubShowHeight-1;
	ShowWidth = SubShowWidth;
	ShowHeight = SubShowHeight;
	/*
	*	初始化数字摄像机硬件状态，用户也可以在其他位置初始化数字摄像机，
	*	但应保证数字摄像机已经打开，建议用户在应用程序初始化时，
	*	同时初始化数字摄像机硬件。
	*/
	//	设置数字摄像机分辨率
	HVSetResolution(m_hhv, Resolution);
	//	采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)
	HVSetSnapMode(m_hhv, SnapMode);
	//  采集速度
	HVSetSnapSpeed(m_hhv,SnapSpeed);
	//  设置各个分量的增益
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
	//	设置曝光时间
	SetExposureTime(ShowWidth,ExposureTint_Upper,ExposureTint_Lower);
	//  设置ADC的级别
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);
	/*
	*	视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，
	*  视频窗口左上角X坐标和窗口宽度应为4的倍数，左上角Y坐标和窗口高度应为2的倍数
	*	输出窗口的起始位置一般设置为(0, 0)即可。
	*/
	HVSetOutputWindow(m_hhv, ShowStartX, ShowStartY, ShowWidth, ShowHeight);
	//	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth			= ShowWidth;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo->bmiHeader.biHeight			= ShowHeight;
	/*
	*	以下设置一般相同，
	*	对于低于8位的位图，还应设置相应的位图调色板
	*/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	//	分配原始图像缓冲区，一般用来存储采集图像原始数据
	//	一般图像缓冲区大小由输出窗口大小和视频格式确定。
	m_pRawBuffer = new BYTE[ShowWidth * ShowHeight];
	ASSERT(m_pRawBuffer);
	//用来进行模板测试的缓冲区
	m_pPatternBuf = new int[ShowWidth * ShowHeight];
	memset(m_pPatternBuf,0,ShowWidth * ShowHeight * sizeof(int));
	//用来记录坏点,
	m_pBadPixel  = new BYTE[ShowWidth * ShowHeight];
	//分配Bayer转换后图像数据缓冲
	m_pImageBuffer = new BYTE[ShowWidth * ShowHeight * 3];
	ASSERT(m_pImageBuffer);
	return TRUE;
}

void MyHVControl::HVDestory(void)
{
	if(Flag_HVControl == FALSE)
		return;
	if(m_bStart == TRUE)
		HVEnd();
	EndHVDevice(m_hhv);
	HVDeviceNum = -1;
	delete []m_pRawBuffer;
	delete []m_pPatternBuf;
	delete []m_pBadPixel;
	delete []m_pImageBuffer;
}

BOOL MyHVControl::HVStart(HV_SNAP_PROC SubProc, HWND m_SubhWnd)
{
	if(Flag_HVControl == FALSE)
		return FALSE;

	status = STATUS_OK;

	MypParam.m_hWnd = m_SubhWnd;
	MypParam.pParam = HVDeviceNum;

	/*
	*	初始化数字摄像机采集图像到内存的控制，
	*	指定回调函数SnapThreadCallback和用户参数m_hWnd
	*/
	status = HVOpenSnap(m_hhv, SubProc, &MypParam);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
	{
		m_bOpen = TRUE;		//标志已经打开SnapEx环境
	}
	else
	{
		return FALSE;
	}

	/*
	*	启动数字摄像机采集图像到内存
	*/
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hhv, ppBuf ,1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
	{
		m_bStart = TRUE;
	}
	else
	{
		return FALSE;
	}

	Flag_Work = TRUE;

	return TRUE;
}

BOOL MyHVControl::HVEnd(void)
{
	if(Flag_HVControl == FALSE)
		return FALSE;
	/*
	*	终止数字摄像机采集图像到内存，同时释放所有采集环境，
	*	再次启动数字摄像机采集，必须重新初始化
	*/
	status = HVCloseSnap(m_hhv);

	HV_VERIFY(status);
	if (HV_SUCCESS(status))
	{
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
	}
	else
	{
		return FALSE;
	}

	Flag_Work = FALSE;

	return TRUE;
}

void MyHVControl::HVFliter(void)
{
	if(Flag_HVControl == FALSE)
		return;

	status = STATUS_OK;

	if(m_bIsToGetBadPixel)
	{
		//得到坏点位置
		DetectDeadPixel(m_pRawBuffer,m_pBadPixel,ShowWidth,ShowHeight);
		m_bIsToGetBadPixel = FALSE;
	}
	if(m_bIsToRemoveBadPixel)
	{
		//消除坏点
		EraseDeadPixel(m_pRawBuffer,m_pBadPixel,0,0,ShowWidth,ShowHeight,ShowWidth,ShowHeight);
	}

	if(m_bIsToGetTemplate)
	{
		if(m_nTempTimes ==0)  //第一次作的时候必须清零
			memset(m_pPatternBuf,'\0',sizeof(int)*ShowWidth * ShowWidth);
		//取10次模板,然后平均
		GetPatternNoise(m_pRawBuffer,m_pPatternBuf,ShowWidth,ShowHeight);
		m_nTempTimes ++ ;
		if(m_nTempTimes >= 10)
		{
			AvgPatternNoise(m_pPatternBuf,ShowWidth,ShowHeight,m_nTempTimes);
			m_bIsToGetTemplate = FALSE;
		}
	}

	if(m_bIsUseTemplate)
	{
		//使用模板对图像数据进行校正
		FixPatternNoise(m_pRawBuffer,m_pPatternBuf,0,0,ShowWidth,ShowHeight,ShowWidth,ShowHeight);
	}

    //同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,ShowWidth,ShowHeight,ConvertType,m_pLutR,m_pLutG,m_pLutB,true,Layout);


	BYTE *p = NULL, * q = NULL;
	//	进行图像反色
	if (m_bNegative)
	{
		for (int i = 0; i < ShowHeight; i++)
		{
			p = m_pImageBuffer + i * ShowWidth * 3;
			for(int j = 0; j < ShowWidth; j++)
			{
				q = p + j * 3;
				*(q + 0) = ~(*(q + 0));
				*(q + 1) = ~(*(q + 1));
				*(q + 2) = ~(*(q + 2));
			}
		}
	}
}

void MyHVControl::SetGain(long lValue)
{
	for (int i = 0; i < 2; i++)
	{
		HVAGCControl(m_hhv, RED_CHANNEL + i, lValue);
	}
}

//根据卡的其他参数设置曝光时间
//其他的参数如摄像机时钟频率，消隐值都取默认值，
//参数：
//nWindWidth:当前图像宽度
//lTintUpper:曝光时间的分子, lTintUpper/lTintLower 组成实际的曝光时间
//lTintLower:曝光时间的分母，lTintUpper/lTintLower 组成实际的曝光时间
void MyHVControl::SetExposureTime(int nWindWidth,long lTintUpper,long lTintLower)
{
	int size = sizeof(HVTYPE);
	HVTYPE type;
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);
	//When outputwindow changes, change the exposure
	//请参考曝光系数转换公式
	long lClockFreq = 24000000;
	int nOutputWid = nWindWidth;
	double dExposure = 0.0;
	double dTint = max((double)lTintUpper/(double)lTintLower,MY_ZERO);
	if(type == HV1300UCTYPE || type == HV1301UCTYPE)
	{
		long lTb = 0;
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 244.0 + lTb);
	}
	else
	{
		long lTb = 0;
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 305.0 + lTb) + 1;
	}

	if (dExposure > 16383)
		dExposure = 16383;
	HVAECControl(m_hhv, AEC_EXPOSURE_TIME, (long)dExposure);
}
