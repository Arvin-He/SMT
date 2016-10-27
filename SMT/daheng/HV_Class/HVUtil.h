/*
 *-----------------------------------------------------------------------------
 *       Name				HVUtil.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2004/01/16
 *
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVUTIL)         //
#define HV_HVUTIL


/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif

HVSTATUS __stdcall HVSaveJPEG(char *lpFileName, BYTE *pBuffer, int nWidth, int nHeight, int nBitCount, BOOL bVerFlip, int nQuality);
HVSTATUS __stdcall HVLoadJPEG(char *lpFileName, BYTE *pBuffer, int *pWidth, int *pHeight, int *pBitCount, BOOL bVerFlip);		

/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif