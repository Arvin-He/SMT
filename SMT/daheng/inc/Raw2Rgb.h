//-----------------------------------------------------------------------------------
/**
 @file  Raw2Rgb.h
 @brief  		
 
 Copyright (c) 2009, China Daheng Group,Inc. Beijing Image Vision Technology Branch
 Development Kit
 Microsoft Win32 SDK, Visual C++ 6.00 
 
 @Author Xu Yibo, HuJing, Liumin, LuPeng,zhao zuxuan 
 @Date   2011-12-23
 @version V1.7.7.12
*/
//-----------------------------------------------------------------------------------

#ifndef __RAW_TO_RGB
#define __RAW_TO_RGB


/** By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#else

typedef char bool;

#endif

#include "HVDef.h"

///定义bayer转换和获取白平衡系数转换码
typedef enum tagHV_CONVERT_CODE { 
	CONV_RAW2RGB = 0,
	CONV_GET_WHITEBALANCE_VALUE = 1
} HV_CONVERT_CODE;

///定义贝尔转换类型
typedef enum tagHV_BAYER_CONVERT_TYPE
{
	BAYER2RGB_ZHC,  
	BAYER2RGB_NEIGHBOUR,  
	BAYER2RGB_BILINER,    
	BAYER2RGB_PATTERN,       
	BAYER2RGB_EDGESENSING,  
	BAYER2RGB_RF,          
	BAYER2RGB_PIXELGROUPING, 
	BAYER2RGB_NEIGHBOUR1,
	BAYER2RGB_ADAPTIVE,	 
	BAYER2RGB_RESERVERED2,	 
	BAYER2RGB_RESERVERED3	
		
} HV_BAYER_CONVERT_TYPE;

///定义bayer2rgb参数结构体
typedef struct tagHV_CONVERT_RAW2RGB {	
	BYTE   *pDestData;
	HV_COLOR_CODE   ccDestColorCode;
	DWORD  dwDestBitCount;
	DWORD  dwDestDataDepth;
	BYTE   *pSrceData;
	HV_COLOR_CODE   ccSrceColorCode;
	DWORD  dwSrceBitCount;
	DWORD  dwSrceDataDepth;
	HV_COLOR_FILTER cfFilter;
	int    nWidth;
	int    nHeight;
	HV_BAYER_CONVERT_TYPE cvType;
	BYTE   *pLutR;
	BYTE   *pLutG;
	BYTE   *pLutB;
	bool   bIsFlip;
	
}HV_CONVERT_RAW2RGB, *PHV_CONVERT_RAW2RGB;

///定义获取白平衡参数结构体
typedef struct tagHV_GET_WHITEBALANCE_VALUE {	
	BYTE    *pData;
	HV_COLOR_CODE ccColorCode;
	DWORD   dwBitCount;
	DWORD   dwDataDepth;
	int     nWidth;
	int     nHeight;
	double  *dValueR;
	double  *dValueG;
	double  *dValueB;
	
}HV_GET_WHITEBALANCE_VALUE, *PHV_GET_WHITEBALANCE_VALUE;


///定义bayer类型
typedef enum tagHV_BAYER_LAYOUT
{
	BAYER_GB = 1,    ///< start from GB
	BAYER_GR = 2,    ///< start from GR
	BAYER_BG = 3,    ///< start from BG
	BAYER_RG = 4     ///< start from RG
} HV_BAYER_LAYOUT;


///定义光照类型
typedef enum tagHV_ILLUMINATE
{
	DAYLIGHT,
	FLUORESCENT,
	INCANDESCENT,
}HV_ILLUMINANT;

//--------------------------------------------------
/**
\brief  Convert Raw12Packed to Raw16
\param  pInBuf   [in]The input Buffer.      
\param  pOutBuf  [out]The output Buffer(new buffer).  
\param  iWidth   [in]The width of image.    
\param  iHeight  [in]The height of image.
return   void  
*/
//--------------------------------------------------
void __stdcall Raw12PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);


//-------------------------------------------------
/**
\brief  Convert Raw10Packed to Raw16
\param  pInBuf   [in]The input Buffer.      
\param  pOutBuf  [out]The output Buffer(new buffer).  
\param  iWidth   [in]The width of image.    
\param  iHeight  [in]The height of image.
return  void
*/
//-------------------------------------------------
void __stdcall Raw10PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);


//-------------------------------------------------
/**
\brief   Color Correction for AOI(24位RGB图像)
\param   pImg		  [in,out]RGB Image Buffer
\param   roi_left	  [in]Left of ROI in Pixel
\param   roi_top	  [in]Top of ROI 
\param   roi_wid	  [in]Width of ROI
\param   roi_hei	  [in]Height of ROI
\param   nImgWid	  [in]Width of Image
\param   nImgHei	  [in]Height of Image
return   void 
*/
//-------------------------------------------------
void __stdcall ROIColorCorrection( void *pImg, int roi_left, int roi_top, int roi_wid, int roi_hei, int nImgWid, int nImgHei);

//------------------------------------------------
/**
\brief  8位图像顺时针旋转90度
\param  pInputBuffer  [in]输入图像缓冲区
\param  Width         [in]图像宽度
\param  nHeight       [in]图像高度
\param  pOutputBuffer [out]输出图像缓冲区（new buffer）
return  状态参数
*/
//------------------------------------------------
HVSTATUS __stdcall HVRotate90CW8B(const BYTE* pInputBuffer, int nWidth, int nHeight, BYTE* pOutputBuffer);

//------------------------------------------------
/**
\brief  8位图像逆时针旋转90度
\param  pInputBuffer    [in]输入图像缓冲区
\param  nWidth          [in]图像宽度
\param  nHeight         [in]图像高度
\param  pOutputBuffer   [out]输出图像缓冲区（new buffer）
return  状态参数
*/
//------------------------------------------------
HVSTATUS __stdcall HVRotate90CCW8B(const BYTE* pInputBuffer, int nWidth, int nHeight, BYTE* pOutputBuffer);

//-----------------------------------------------
/**
\brief  图像亮度调节(24位RGB图像或者8位灰度图像)
\param  pInputBuffer  [in]输入图像缓冲区      
\param  nImagesize    [in]原始图像的大小，以字节为单位(RGB:图像宽x图像高x3)
\param  factor        [in]调节因子，范围：-150~150
\param  pOutputBuffer [out]输出图像缓冲区
return  状态参数
*/
//-----------------------------------------------
HVSTATUS __stdcall HVBrightness(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  图像对比度调节(24位RGB图像或者8位灰度图像)
\param  pInputBuffer	[in]输入图像缓冲区      
\param  nImagesize		[in]原始图像的大小，以字节为单位(RGB:图像宽x图像高x3)
\param  factor			[in]调节因子，范围：-50~100
\param  pOutputBuffer	[out]输出图像缓冲区
return  状态参数
*/
//--------------------------------------------------
HVSTATUS __stdcall HVContrast(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  24位RGB图像锐化
\param  pInputBuffer  [in]输入图像缓冲区      
\param  nWidth        [in]图像宽度
\param  nHeight       [in]图像高度
\param  factor        [in]调节因子，范围：0.1~5.0
\param  pOutputBuffer [out]输出图像缓冲区
return  状态参数
*/
//--------------------------------------------------
HVSTATUS __stdcall HVSharpen24B(BYTE* pInputBuffer,int nWidth,int nHeight,float factor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  24位RGB图像饱和度调节函数
\param  pOutputBuffer  [out]数据输出buffer指针
\param  pInputBuffer   [in]数据输入buffer指针
\param  nImageSize     [in]RGB图像尺寸 (图像宽x图像高)
\param  nFactor        [in]调节参数,范围：0 ~ 128
return  状态参数   
*/
//--------------------------------------------------
HVSTATUS __stdcall HVSaturation(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  bayer图像转换成RGB图像函数
\param  pDest   [out]数据输出buffer指针（24位RGB图像）
\param  pSrce   [in]数据输入buffer指针（8位Raw图像）
\param  nWid    [in]图像宽
\param  nHei    [in]图像高
\param  cvtype  [in]插值算法类型
\param  pLutR   [in]R分量数字增益查找表
\param  pLutG   [in]G分量数字增益查找表
\param  pLutB   [in]B分量数字增益查找表
\param  bFlip   [in]图像翻转标记 true:翻转 false:不翻转
\param  Layout  [in]bayer类型
return  void  
*/
//--------------------------------------------------
void __stdcall ConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,BYTE* pLutR,BYTE* pLutG, BYTE* pLutB,bool bFlip,HV_BAYER_LAYOUT Layout);


//--------------------------------------------------
/**
\brief  获取白平衡系数函数，为了计算准确，相机照客观“白”区域，或者传入图像为“白色区域”
\param  pSrce   [in]数据输入buffer指针（24bitRGB图像）
\param  nWid    [in]图像宽
\param  nHei    [in]图像高
\param  dRatioR [out]R分量白平衡系数
\param  dRatioG [out]G分量白平衡系数
\param  dRatioB [out]B分量白平衡系数
return  void  
*/
//--------------------------------------------------
void __stdcall GetWhiteBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);

//--------------------------------------------------
/**
\brief 设置Gamma查找表函数
\param  pLutGamma[256]   [out]计算出的8位图像Gamma查找表
\param  dGammaRatio      [in]Gamma系数   (0.1 ~ 10.0)
return  void  
*/
//--------------------------------------------------
void __stdcall SetGammaLut(BYTE pLutGamma[256],double dGammaRatio);


//-----------------------------------------------------
/**
\brief  8位Raw图像坏点检测函数，检测时必须盖上镜头盖
\param  pRawImgBuf         [in]8位Raw图输入buffer指针
\param  pBadPixelPosBuf    [out]存放坏点位置的buffer指针
\param  nImgWid            [in]图像宽
\param  nImgHei            [in]图像高
return  void  
*/
//-----------------------------------------------------
void __stdcall DetectDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nImgWid,int nImgHei);


//-----------------------------------------------------
/**
\brief  8位Raw图像坏点校正函数
\param  pRawImgBuf         [in,out]8位Raw图输入buffer指针
\param  pBadPixelPosBuf    [in]存放坏点位置的buffer指针(DetectDeadPixel函数的输出)
\param  nXPos              [in]图像起始位置X坐标(相对坏点模板)
\param  nYpos              [in]图像起始位置Y坐标(相对坏点模板)
\param  nImgWid            [in]图像宽
\param  nImgHei            [in]图像高
\param  nBadPixelBufWid    [in]保存坏点位置信息的缓冲区的宽
\param  nBadPixelBufHei    [in]保存坏点位置信息的缓冲区的高
return  void  
*/
//-----------------------------------------------------
void __stdcall EraseDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nBadPixelBufWid,int nBadPixelBufHei);


//-----------------------------------------------------
/**
\brief  Raw图像坏点检测函数,该方法只能检测黑图（盖上镜头），支持8 ~ 16 位数据,这里，位数指实际位数，
        当超过8位时，数据占两个字节，但实际有效位数可能是 9 ~ 16的任意数，对于Packed格式，需先转换成raw16
\param  pRawImgBuf         [in]Raw图输入buffer指针
\param  pBadPixelPosBuf    [out]存放坏点位置的buffer指针,buffer大小与图像大小同
\param  nImgWid            [in]图像宽
\param  nImgHei            [in]图像高
\param  nBitNum            [in]图像数据的实际位数（注：若是10位数据则输入10，12位数据输入12，以此类推，值范围8 ~ 16），参考说明书
return  状态码 
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVDetectRawDeadPos(BYTE *pRawImgBuf,BYTE *pBadPixelPosBuf,int nImgWid,int nImgHei,int nBitNum);


//-----------------------------------------------------
/**
\brief  Raw图像坏点校正函数,支持8 ~ 16 位数据,这里，位数指实际位数，当超过8位时，
        数据占两个字节，但实际有效位数可能是 9 ~ 16的任意数,对于Packed格式，需先转换成raw16
\param  pRawImgBuf         [in,out]Raw图输入buffer指针
\param  pBadPixelPosBuf    [in]存放坏点位置的buffer指针(HVDetectRawDeadPos函数的输出)
\param  nImgWid            [in]图像宽
\param  nImgHei            [in]图像高
\param  nBitNum            [in]图像数据的实际位数（注：若是10位数据则输入10，12位数据输入12，以此类推，值范围8 ~ 16），参考说明书
return  状态码  
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVCorrectRawBadPos(BYTE *pRawImgBuf,BYTE *pBadPixelPosBuf,int nImgWid,int nImgHei,int nBitNum);

//-----------------------------------------------------
/**
\brief  Raw图像自动坏点检测校正函数,支持8 ~ 16 位数据，这里，位数指实际位数，当超过8位时，数据占两个字节，
        但实际有效位数可能是 9 ~ 16的任意数，该函数进行适时检测校正，每帧均需调用,对于Packed格式，需先转换成raw16
\param  pRawImgBuf         [in,out]Raw图输入buffer指针
\param  nImgWid            [in]图像宽
\param  nImgHei            [in]图像高
\param  nBitNum            [in]图像数据的实际位数（注：若是10位数据则输入10，12位数据输入12，以此类推，值范围8 ~ 16），参考说明书
return  状态码  
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVAutoRawDefectivePixelCorrect(BYTE *pRawImgBuf,int nImgWid,int nImgHei,int nBitNum);

//-----------------------------------------------------
/**
\brief  根据输入图像获取噪声模板(处理8位Raw图)
\param  pRawImgBuf     [in]8位Raw图输入buffer指针
\param  pPatternBuf    [out]保存模板信息的缓冲区
\param  nPatternWid    [in]图像宽
\param  nPatternHei    [in]图像高
return  void  
*/
//-----------------------------------------------------
void __stdcall GetPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nPatternWid,int nPatternHei);

//-----------------------------------------------------
/**
\brief  对多次取的模板取平均
\param  pPatternBuf    [in]保存模板信息的缓冲区
\param  nPatternWid    [in]图像宽(模板宽)
\param  nPatternHei    [in]图像高(模板高)
\param  nAvgTimes      [in]平均次数
return  void  
*/
//-----------------------------------------------------
void __stdcall AvgPatternNoise(int * pPatternBuf,int nPatternWid,int nPatternHei,int nAvgTimes);

//-----------------------------------------------------
/**
\brief  用取得的模板对图像进行校正
\param  pRawImgBuf     [in,out]要进行坏点消除处理的8位Raw图像数据
\param  pPatternBuf    [in]保存模板信息的缓冲区
\param  nXPos          [in]图像起始位置x坐标(相对模板)
\param  nYPos          [in]图像起始位置y坐标(相对模板)
\param  nImgWid        [in]图像宽
\param  nImgHei        [in]图像高
\param  nPatternWid    [in]模板缓冲区的宽
\param  nPatternHei    [in]模板缓冲区的高
return  void  
*/
//-----------------------------------------------------
void __stdcall FixPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nPatternWid,int nPatternHei);

//-----------------------------------------------------
/**
\brief  设置坏点检测阈值
\param  [in]BadPixelThreshold   坏点检测阈值   
return  void  
*/
//-----------------------------------------------------
void __stdcall SetBadPixelThreshold(int BadPixelThreshold);

//-----------------------------------------------------
/**
\brief  图像数据转换函数
\param  ConvertCode   [in]功能代码，参见HV_CONVERT_CODE
\param  pContext      [in,out]指向ConvertCode的参数缓冲区
\param  pLength       [in]pContext指向的缓冲区的大小，单位：字节
return  void  
*/
//-----------------------------------------------------
void __stdcall HVConvert(HV_CONVERT_CODE ConvertCode, void *pContext, int *pLength);


#ifdef __cplusplus
}
#endif

#endif


