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

using namespace cv;
#pragma comment(lib, "Vfw32.lib")

//�Զ�����ϢID��
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
	BOOL InitDMC3000Card(); // ��ʼ���˶����ƿ�
	BOOL ResetDMC3000Card();
	//Mat m_src;  

////////////////////dmc�˶����ƿ���ر���///////////////////
private:
	WORD	m_nCard;
////////////////////dmc�˶����ƿ���ر���///////////////////

///////���CCD��ز����ͺ���///////////////////////////////
private:
	HHV	 m_hhv;			///< ������������	
	BOOL m_bOpen;			///< ��ʼ����־
	BOOL m_bStart;			///< ������־
	long m_lHBlanking;		///< ˮƽ����
	long m_lVBlanking;		///< ��ֱ����
	HV_BAYER_LAYOUT m_Layout; ///< bayer��ʽ	
	BITMAPINFO *m_pBmpInfo;	///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE *m_pRawBuffer;		///< �ɼ�ͼ��ԭʼ���ݻ�����
	BYTE *m_pImageBuffer;		///< Bayerת���󻺳���
	char m_chBmpBuf[2048];		///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
	BYTE m_pLutR[256] ;		///< ��ɫ���ұ�,R����
	BYTE m_pLutG[256] ;		///< ��ɫ���ұ�,G����
	BYTE m_pLutB[256] ;		///< ��ɫ���ұ�,B����
	int  m_nError;				///< �������
	int  m_nLost;				///< ��֡����
	int  dwLastStatus;
	/// �ɼ��ص��������û�Ҳ���Զ���Ϊȫ�ֺ����������Ϊ��ĳ�Ա����������Ϊ��̬��Ա������
	static int CALLBACK SnapThreadCallback(HV_SNAP_INFO *pInfo); 
	/// ��ȡ״̬��
	HVSTATUS GetLastStatus();
	///¼ȡ��Ƶ���߳�
	static UINT StoreVideoThreadFunc(LPVOID lpParam);
	void StoreVideo();
	/// ��ȡbayer��ʽ
	HV_BAYER_LAYOUT HVGetBayerType(HHV hhv);
	/// �����ع�ʱ��
	HVSTATUS SetExposureTime(HHV hhv, int nWindWidth, long lTintUpper, long lTintLower, 
		                     long HBlanking, HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution);
///////���CCD��ز���/////////////////////////////////////
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
		
private:
	BOOL m_bIsCapture;		//��Ƶ�����ʶ

public:
	void ShowImage(Mat img, INT_PTR ID);
	void DrawCross(Mat img);
	void DrawImgScale(Mat img);
	void DrawLine(Mat img);
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
};
