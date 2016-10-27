// MyHVControl.h: interface for the MyHVControl class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#if !defined(AFX_MYHVCONTROL_H__5D718F5B_0E81_4BF6_B0A9_4E3F08250471__INCLUDED_)
#define AFX_MYHVCONTROL_H__5D718F5B_0E81_4BF6_B0A9_4E3F08250471__INCLUDED_

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#include "HVDAILT.h"
#include "Raw2Rgb.h"


//�Զ�����ϢID��
#define WM_SNAP_CHANGE		(WM_USER + 100)

//�Զ��峣��
#define  MY_ZERO 0.0000000001
#define  PI 3.1416

//�ɵ�������

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct HVpParam
{
	HWND m_hWnd;
	int pParam;
};

class MyHVControl
{
public:
	MyHVControl();
	virtual ~MyHVControl();

public:
	HV_RESOLUTION Resolution;
	HV_SNAP_MODE SnapMode;
	HV_SNAP_SPEED SnapSpeed;
	HV_BAYER_LAYOUT Layout;
	HV_BAYER_CONVERT_TYPE ConvertType;
	long Gain;
	long ExposureTint_Upper;
	long ExposureTint_Lower;

	long ShutterDelay;
	long ADCLevel;

	int ShowStartX;
	int ShowStartY;
	int ShowEndX;
	int ShowEndY;
	int ShowWidth;
	int ShowHeight;

public:

	/************************************************************************/
	/*                          ������������                              */
	/************************************************************************/

	int HVDeviceNum;	//������������
	HHV	m_hhv;			//������������

	BOOL m_bOpen;		//��ʼ����־
	BOOL m_bStart;		//������־

	BITMAPINFO *m_pBmpInfo;		//BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE *m_pRawBuffer;			//�ɼ�ͼ��ԭʼ���ݻ�����
	BYTE *m_pImageBuffer;		//Bayerת���󻺳���
    char m_chBmpBuf[2048];		//BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����

	int *m_pPatternBuf;     //��������ģ����ԵĻ�����
	int m_nTempTimes;       //ȡģ��Ĵ���
	BYTE *m_pBadPixel;

	BOOL m_bIsToGetBadPixel;
	BOOL m_bIsToRemoveBadPixel;
	BOOL m_bIsToGetTemplate;
	BOOL m_bIsUseTemplate;

	BOOL m_bNegative;

	//��ɫ���ұ�
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;

	HVSTATUS status;		//�������ؽ��

	HVpParam MypParam;

	int Flag_HVControl;
	int Flag_Work;

public:

	void SetExposureTime(int nWindWidth,long lTintUpper,long lTintLower);

	BOOL HVInitial(int SubHVNum,int SubShowStartX,int SubShowStartY,int SubShowWidth,int SubShowHeight);
	BOOL HVInitial(int SubHVNum,int SubShowWidth,int SubShowHeight);
	void HVDestory(void);

	BOOL HVStart(HV_SNAP_PROC SubProc, HWND m_SubhWnd);
	BOOL HVEnd(void);

	void HVFliter(void);

	void SetGain(long lValue);
};

#endif // !defined(AFX_MYHVCONTROL_H__5D718F5B_0E81_4BF6_B0A9_4E3F08250471__INCLUDED_)
