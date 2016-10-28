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

	//	�����������
	HVDeviceNum = SubHVNum;
	status = BeginHVDevice(HVDeviceNum, &m_hhv);
	//	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��
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
	*	��ʼ�����������Ӳ��״̬���û�Ҳ����������λ�ó�ʼ�������������
	*	��Ӧ��֤����������Ѿ��򿪣������û���Ӧ�ó����ʼ��ʱ��
	*	ͬʱ��ʼ�����������Ӳ����
	*/
	//	��������������ֱ���
	HVSetResolution(m_hhv, Resolution);
	//	�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)
	HVSetSnapMode(m_hhv, SnapMode);
	//  �ɼ��ٶ�
	HVSetSnapSpeed(m_hhv,SnapSpeed);
	//  ���ø�������������
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
	//	�����ع�ʱ��
	SetExposureTime(ShowWidth,ExposureTint_Upper,ExposureTint_Lower);
	//  ����ADC�ļ���
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);
	/*
	*	��Ƶ������ڣ�����Ƶ�����Χ���������ȡֵ��Χ���������봰�ڷ�Χ���ڣ�
	*  ��Ƶ�������Ͻ�X����ʹ��ڿ��ӦΪ4�ı��������Ͻ�Y����ʹ��ڸ߶�ӦΪ2�ı���
	*	������ڵ���ʼλ��һ������Ϊ(0, 0)���ɡ�
	*/
	HVSetOutputWindow(m_hhv, ShowStartX, ShowStartY, ShowWidth, ShowHeight);
	//	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	//	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	//	ͼ���ȣ�һ��Ϊ������ڿ��
	m_pBmpInfo->bmiHeader.biWidth			= ShowWidth;
	//	ͼ���ȣ�һ��Ϊ������ڸ߶�
	m_pBmpInfo->bmiHeader.biHeight			= ShowHeight;
	/*
	*	��������һ����ͬ��
	*	���ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��
	*/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	//	����ԭʼͼ�񻺳�����һ�������洢�ɼ�ͼ��ԭʼ����
	//	һ��ͼ�񻺳�����С��������ڴ�С����Ƶ��ʽȷ����
	m_pRawBuffer = new BYTE[ShowWidth * ShowHeight];
	ASSERT(m_pRawBuffer);
	//��������ģ����ԵĻ�����
	m_pPatternBuf = new int[ShowWidth * ShowHeight];
	memset(m_pPatternBuf,0,ShowWidth * ShowHeight * sizeof(int));
	//������¼����,
	m_pBadPixel  = new BYTE[ShowWidth * ShowHeight];
	//����Bayerת����ͼ�����ݻ���
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
	*	��ʼ������������ɼ�ͼ���ڴ�Ŀ��ƣ�
	*	ָ���ص�����SnapThreadCallback���û�����m_hWnd
	*/
	status = HVOpenSnap(m_hhv, SubProc, &MypParam);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
	{
		m_bOpen = TRUE;		//��־�Ѿ���SnapEx����
	}
	else
	{
		return FALSE;
	}

	/*
	*	��������������ɼ�ͼ���ڴ�
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
	*	��ֹ����������ɼ�ͼ���ڴ棬ͬʱ�ͷ����вɼ�������
	*	�ٴ���������������ɼ����������³�ʼ��
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
		//�õ�����λ��
		DetectDeadPixel(m_pRawBuffer,m_pBadPixel,ShowWidth,ShowHeight);
		m_bIsToGetBadPixel = FALSE;
	}
	if(m_bIsToRemoveBadPixel)
	{
		//��������
		EraseDeadPixel(m_pRawBuffer,m_pBadPixel,0,0,ShowWidth,ShowHeight,ShowWidth,ShowHeight);
	}

	if(m_bIsToGetTemplate)
	{
		if(m_nTempTimes ==0)  //��һ������ʱ���������
			memset(m_pPatternBuf,'\0',sizeof(int)*ShowWidth * ShowWidth);
		//ȡ10��ģ��,Ȼ��ƽ��
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
		//ʹ��ģ���ͼ�����ݽ���У��
		FixPatternNoise(m_pRawBuffer,m_pPatternBuf,0,0,ShowWidth,ShowHeight,ShowWidth,ShowHeight);
	}

    //ͬʱ��ԭʼ���ݽ������·�ת
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,ShowWidth,ShowHeight,ConvertType,m_pLutR,m_pLutG,m_pLutB,true,Layout);


	BYTE *p = NULL, * q = NULL;
	//	����ͼ��ɫ
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

//���ݿ����������������ع�ʱ��
//�����Ĳ����������ʱ��Ƶ�ʣ�����ֵ��ȡĬ��ֵ��
//������
//nWindWidth:��ǰͼ����
//lTintUpper:�ع�ʱ��ķ���, lTintUpper/lTintLower ���ʵ�ʵ��ع�ʱ��
//lTintLower:�ع�ʱ��ķ�ĸ��lTintUpper/lTintLower ���ʵ�ʵ��ع�ʱ��
void MyHVControl::SetExposureTime(int nWindWidth,long lTintUpper,long lTintLower)
{
	int size = sizeof(HVTYPE);
	HVTYPE type;
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);
	//When outputwindow changes, change the exposure
	//��ο��ع�ϵ��ת����ʽ
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
