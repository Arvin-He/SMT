/*
 *-----------------------------------------------------------------------------
 *       Name				HVDAILT.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2004/04/16
 *		 Version			1.01.0002
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVDIALT)         //
#define HV_HVDIALT

#include "HVDef.h"

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
HVSTATUS __stdcall BeginHVDevice(int nDevice, HHV *pHandle);
HVSTATUS __stdcall EndHVDevice(HHV hhv);

HVSTATUS __stdcall HVSetResolution(HHV hhv, HV_RESOLUTION resolution);
HVSTATUS __stdcall HVSetOutputWindow(HHV hhv, int nXStart, int nYStart, int nWidth, int nHeight);
HVSTATUS __stdcall HVSetBlanking(HHV hhv, int nHor, int nVert);
HVSTATUS __stdcall HVSetSnapMode(HHV hhv, HV_SNAP_MODE mode);
HVSTATUS __stdcall HVSetTriggerPolarity(HHV hhv, HV_POLARITY polarity);
HVSTATUS __stdcall HVSetStrobePolarity(HHV hhv, HV_POLARITY polarity);
HVSTATUS __stdcall HVSetSnapSpeed(HHV hhv, HV_SNAP_SPEED speed);
HVSTATUS __stdcall HVEnableVideoMirror(HHV hhv, HV_MIRROR_DIRECTION dir, BOOL bEnable);

HVSTATUS __stdcall HVADCControl(HHV hhv, BYTE byParam, long lValue);
HVSTATUS __stdcall HVAGCControl(HHV hhv, BYTE byParam, long lValue);
HVSTATUS __stdcall HVAECControl(HHV hhv, BYTE byParam, long lValue);

HVSTATUS __stdcall HVSnapShot(HHV hhv, BYTE **ppBuffer, int nSum);
HVSTATUS __stdcall HVOpenSnap(HHV hhv, HV_SNAP_PROC pSnapFunc, void *pParam);
HVSTATUS __stdcall HVCloseSnap(HHV hhv);
HVSTATUS __stdcall HVStartSnap(HHV hhv, BYTE **ppBuffer, int nSum);
HVSTATUS __stdcall HVStopSnap(HHV hhv);
HVSTATUS __stdcall HVTriggerShot(HHV hhv);
HVSTATUS __stdcall HVWaitSync(HHV hhv);

char * __stdcall HVGetErrorString(HVSTATUS status);
HVSTATUS __stdcall HVGetDeviceTotal(int *pNumber);
HVSTATUS __stdcall HVGetDeviceInfo(HHV hhv, HV_DEVICE_INFO param, void *pContext, int *pSize);
HVSTATUS __stdcall HVCheckDevice(HHV hhv, HV_CHECK_PARAM param, BOOL *pStatus);

HVSTATUS __stdcall HVDeviceMemRead(HHV hhv, DWORD dwOffset, BYTE *pBuffer, DWORD dwLength);
HVSTATUS __stdcall HVDeviceMemWrite(HHV hhv, DWORD dwOffset, BYTE *pBuffer, DWORD dwLength);

HVSTATUS __stdcall HVCommand(HHV hhv, HV_COMMAND_CODE CommandCode, void *pContext, int *pLength);

/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif