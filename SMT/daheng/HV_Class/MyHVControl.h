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


//自定义消息ID号
#define WM_SNAP_CHANGE		(WM_USER + 100)

//自定义常数
#define  MY_ZERO 0.0000000001
#define  PI 3.1416

//可调整变量

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
	/*                          数字摄像机相关                              */
	/************************************************************************/

	int HVDeviceNum;	//数字摄像机序号
	HHV	m_hhv;			//数字摄像机句柄

	BOOL m_bOpen;		//初始化标志
	BOOL m_bStart;		//启动标志

	BITMAPINFO *m_pBmpInfo;		//BITMAPINFO 结构指针，显示图像时使用
	BYTE *m_pRawBuffer;			//采集图像原始数据缓冲区
	BYTE *m_pImageBuffer;		//Bayer转换后缓冲区
    char m_chBmpBuf[2048];		//BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区

	int *m_pPatternBuf;     //用来进行模板测试的缓冲区
	int m_nTempTimes;       //取模板的次数
	BYTE *m_pBadPixel;

	BOOL m_bIsToGetBadPixel;
	BOOL m_bIsToRemoveBadPixel;
	BOOL m_bIsToGetTemplate;
	BOOL m_bIsUseTemplate;

	BOOL m_bNegative;

	//颜色查找表
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;

	HVSTATUS status;		//函数返回结果

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
