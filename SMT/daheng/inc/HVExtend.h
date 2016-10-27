//----------------------------------------------------------------------------------
/**
	\file		HVExtend.h
	\brief		the declaration of the Main interface for the HVExtend.dll module.
	\version	V1.0.0.2
	\date		2011-08-09
*/
//----------------------------------------------------------------------------------

#if !defined(H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_)
#define H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_

#include "HVDef.h"
#include "Raw2Rgb.h"

#ifdef __cplusplus
extern "C" {
#endif

///定义白平衡比例系数
typedef struct tagHV_WB_RATIO
{
	double dRatioR;     ///<白平衡红分量比例系数
	double dRatioB;     ///<白平衡蓝分量比例系数
}HV_WB_RATIO;

// 初始化HVExtend 接口需要的资源
HVSTATUS __stdcall HVInitialize(HHV hhv);

// 清除HVExtend接口需要的资源
HVSTATUS __stdcall HVUnInitialize(HHV hhv);

// 开始视频预览
HVSTATUS __stdcall HVStartPreview(HHV hhv,
								  int nImgWid,
								  int nImgHei,
								  HV_ADC_LEVEL nLevel,
								  LPCSTR lpszTitle,
								  DWORD style,
								  int x,
								  int y,
								  int nWid,
								  int nHei,
								  HWND hwndParent,
								  HMENU hMenu);

// 暂停视频预览
HVSTATUS __stdcall HVPausePreview(HHV hhv, BOOL bPause);

// 停止视频预览
HVSTATUS __stdcall HVStopPreview(HHV hhv);

// Bayer图像转换为RGB图像
HVSTATUS __stdcall HVBayerToRGB(HHV  hhv,
								BYTE *pSrcBuf,
								int  nWidth,
								int  nHeight,
								bool bIsFlip,
								HV_ADC_LEVEL nLevel,
								HV_BAYER_CONVERT_TYPE cvtype,
								BYTE *pDestBuf,
								HV_WB_RATIO *pAWBRatio);



#ifdef __cplusplus
}
#endif

#endif // !defined(H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_)

