// SMTDlg.h : header file
#pragma once
#include <vfw.h>
#include "daheng/inc/HVDAILT.h"
#include "daheng/inc/Raw2Rgb.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "afxcmn.h"

#include "ManualDlg.h"
#include "SemiAutoDlg.h"
#include "ImageAssistDlg.h"
#include "SettingDlg.h"
#include "SetParamDlg.h"
#include <vector>
using namespace std;
using namespace cv;
#pragma comment(lib, "Vfw32.lib")

//自定义消息ID号
#define WM_SNAP_CHANGE		(WM_USER + 100)
#define WM_SNAP_ERROR		(WM_USER + 101) 
#define WM_SNAP_STOP		(WM_USER + 102)

// CSMTDlg dialog
class CSMTDlg : public CDialogEx
{
// Construction
public:
	CSMTDlg(CWnd* pParent = NULL);	// standard constructor
	~CSMTDlg();
	BOOL InitDMC3000Card(); // 初始化运动控制卡
	BOOL ResetDMC3000Card();
	void InitDMC3000Status();
	void SetDMC3000Status(BOOL status, int nID);
	void UpdateDMC3000Data();
	void UpdateDMC3000Status();
	void UpdateDMC3000PulseAndDistance(int nAxisIndex, int nPulseID, int nDisID);
	void UpdateDMC3000Status(int nAxisIndex, int elupID, int eldownID, int orgID);
private:
	CString m_strPulseCount;
	CString m_strDistance;
	//Mat m_src;  

///////大恒CCD相关参数和函数///////////////////////////////
private:
	HHV	 m_hhv;			///< 数字摄像机句柄	
	BOOL m_bOpen;			///< 初始化标志
	BOOL m_bStart;			///< 启动标志
	long m_lHBlanking;		///< 水平消隐
	long m_lVBlanking;		///< 垂直消隐
	HV_BAYER_LAYOUT m_Layout; ///< bayer格式	
	BITMAPINFO *m_pBmpInfo;	///< BITMAPINFO 结构指针，显示图像时使用
	BYTE *m_pRawBuffer;		///< 采集图像原始数据缓冲区
	BYTE *m_pImageBuffer;		///< Bayer转换后缓冲区
	char m_chBmpBuf[2048];		///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	BYTE m_pLutR[256] ;		///< 颜色查找表,R分量
	BYTE m_pLutG[256] ;		///< 颜色查找表,G分量
	BYTE m_pLutB[256] ;		///< 颜色查找表,B分量
	int  m_nError;				///< 错误计数
	int  m_nLost;				///< 丢帧计数
	int  dwLastStatus;
	/// 采集回调函数，用户也可以定义为全局函数，如果作为类的成员函数，必须为静态成员函数。
	static int CALLBACK SnapThreadCallback(HV_SNAP_INFO *pInfo); 
	/// 获取状态码
	HVSTATUS GetLastStatus();
	///录取视频的线程
	static UINT StoreVideoThreadFunc(LPVOID lpParam);
	void StoreVideo();
	/// 获取bayer格式
	HV_BAYER_LAYOUT HVGetBayerType(HHV hhv);
	/// 设置曝光时间
	HVSTATUS SetExposureTime(HHV hhv, int nWindWidth, long lTintUpper, long lTintLower, 
		                     long HBlanking, HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution);
///////大恒CCD相关参数/////////////////////////////////////
// Dialog Data
	enum { IDD = IDD_SMT_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	BOOL InitialDHCamera();
	BOOL DestroyDHCamera(HHV hDHCamera, BYTE *pRawBuffer, BYTE *pImageBuffer);
	BOOL SetGain(int ctrID);

// Implementation
protected:
	HICON		m_hIcon;
	CMenu		m_menu;
	CToolBar	m_toolBar;
	CImageList	m_imageList;

	CSliderCtrl m_sliderGain;
	CSpinButtonCtrl m_spinGain;
	CSliderCtrl m_sliderShutter;
	CSpinButtonCtrl m_spinShutter;
	int m_editShutter;
	int m_editGain;

	CTabCtrl		m_tab;
	CTabCtrl		m_bottomTab;
	CManualDlg		m_manualDlg;
	CSemiAutoDlg	m_semiAutoDlg;
	CImageAssistDlg m_imageAssistDlg;
	CSettingDlg		m_settingDlg;
	CSetParamDlg    m_setParamDlg;
		
private:
	BOOL m_bIsCapture;		//视频保存标识

public:
	void ShowImage(Mat img, INT_PTR ID);
	void DrawCross(Mat img);
	void DrawImgScale(Mat img);
	void DrawLine(Mat img);
	void DrawRect(Mat img);
	void DrawCircle(Mat img);
	void MeasureDis(Mat img);
	vector <CvPoint> m_drawLinePoints;
	vector <CvPoint> m_drawRectPoints;
	vector <CvPoint> m_drawCirclePoints;
	vector <CvPoint> m_measureDisPoints;
	vector <CvPoint> m_measureAnglePoints;

	//CvPoint m_drawLinePoints[2];
	//CvPoint m_drawRectPoints[3];
	//CvPoint m_drawCirclePoints[2];

	// Gap functionsenerated message m
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSnapChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapStop(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCamera_Open();
	afx_msg void OnCamera_Stop();
	afx_msg void OnCamera_Close();
	afx_msg void OnCamera_SavePic();
	afx_msg void OnCamera_SaveVideo();
	afx_msg void OnCamera_StopVideo();	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickedSetStageXHomeBtn();
	afx_msg void OnClickedSetStageYHomeBtn();
	afx_msg void OnClickedSetCcdXHomeBtn();
	afx_msg void OnClickedSetCcdZHomeBtn();
	afx_msg void OnClickedStageXGohomeBtn();
	afx_msg void OnClickedStageYGohomeBtn();
	afx_msg void OnClickedCcdXGohomeBtn();
	afx_msg void OnClickedCcdZGohomeBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedStageXStopBtn();
	afx_msg void OnClickedStageYStopBtn();
	afx_msg void OnClickedCcdXStopBtn();
	afx_msg void OnClickedCcdZStopBtn();
};
