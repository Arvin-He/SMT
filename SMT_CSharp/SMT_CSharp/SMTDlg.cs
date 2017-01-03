﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.Threading;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.Util;
using USBCamera;
using USBCameraAPI = USBCamera.API;
using DH3151UC;
using DMC3400A;
using csLTDMC;

using SMT_CSharp;

namespace SMT_CSharp
{
    public partial class SMTDlg : Form
    {
#region variable
        DH3151UC.USBDHCamera m_Camera = new DH3151UC.USBDHCamera();
        private HV_SNAP_PROC snapCallback = new HV_SNAP_PROC(SnapCallBack);
        private bool m_bIsSnap = false;
        private bool m_bIsOpen = false;
//         private Capture m_capture = null;
//         private VideoWriter m_videoWriter = null;
//         private bool m_IsCapture = false;
//         private String m_videoPath = null;
//         private Image<Bgr, Byte> frame = null;
        public Image<Bgr, Byte> m_src = null;
        DMC3400A.DMC3400ACard m_dmc3400ACard = new DMC3400A.DMC3400ACard();
#endregion
#region public method

        public SMTDlg()
        {
            InitializeComponent();
            m_Camera.Initialize();
            m_dmc3400ACard.InitDMC3400ACard();
        }

        ~SMTDlg()
        {
            CloseSnap();
            m_Camera.Release();
        }
#endregion

#region private method
        private void OpenCCDBtn_Click(object sender, EventArgs e)
        {
            OpenSnap();
            StartSnap();
            UpdateUI();
        }

        private void StopCCD_Click(object sender, EventArgs e)
        {
            StopSnap();
            UpdateUI();
        }

        private void CloseCCD_Click(object sender, EventArgs e)
        {
            CloseSnap();
            UpdateUI();
        }

        private void SavePic_Click(object sender, EventArgs e)
        {
            SaveFileDialog imageSaveDlg = new SaveFileDialog();
            imageSaveDlg.InitialDirectory = "./data/images";
            imageSaveDlg.Filter = "bmp file(*.bmp)|*.bmp|png file(*.png)|*.png|jpg file(*.jpg)|*.jpg";
            if (imageSaveDlg.ShowDialog() == DialogResult.OK)
            {
                Bitmap saveImage = new Bitmap(640, 480);
                saveImage = m_Camera.GetCurrentBMP().Clone() as Bitmap;
                saveImage.Save(imageSaveDlg.FileName);
            }
        }

        private void SaveVideo_Click(object sender, EventArgs e)
        {
            /*SaveFileDialog videoSaveDlg = new SaveFileDialog();
            videoSaveDlg.InitialDirectory = "./data/videos";
            videoSaveDlg.Filter = "AVI(*.avi)|*.avi";
            videoSaveDlg.AddExtension = true;
            if (videoSaveDlg.ShowDialog() == DialogResult.OK)
            {
                Thread SaveVideoThread = new Thread(new ThreadStart(SaveVideoThreadFunc));
                SaveVideoThread.Start();
                m_videoPath = videoSaveDlg.FileName;                
            }*/
        }

        private void SaveVideoThreadFunc()
        {
            /*m_IsCapture = true;
            m_videoWriter = new VideoWriter(m_videoPath,
                        CvInvoke.CV_FOURCC('M', 'J', 'P', 'G'), 15, 640, 480, true);
            while (m_IsCapture)
            {
                //Bitmap bmp = m_Camera.GetCurrentBMP().Clone() as Bitmap;
                //Image<Bgr, Byte> frame1 = new Image<Bgr, Byte>(bmp);
                m_videoWriter.WriteFrame(m_src);
                CvInvoke.cvWaitKey(27);
                if (!m_IsCapture) break;
            } */           
        }

        private void ProcessFrame(object sender, EventArgs arg)
        {
            /*while (m_IsCapture)
            {
                if (!m_IsCapture)
                {
                    break;
                }
                Bitmap saveImage = new Bitmap(640, 480);
                saveImage = m_Camera.GetCurrentBMP().Clone() as Bitmap;
                saveImage.Save("D:/123.bmp");
                frame = new Image<Bgr, Byte>(m_Camera.GetCurrentBMP().Clone() as Bitmap);
                if (m_videoWriter.Ptr != IntPtr.Zero)
                {
                    m_videoWriter.WriteFrame(frame);
                }                
            } */           
        }

        private void StopSaveVideo_Click(object sender, EventArgs e)
        {
            /*m_IsCapture = false;
            Application.Idle -= ProcessFrame;
            m_videoWriter.Dispose();*/
        }

        private void SaveCCDParam_Click(object sender, EventArgs e)
        {
            
        }

        private void LoadCCDParam_Click(object sender, EventArgs e)
        {

        }
        
        private void UpdateUI()
        {

        }

        private void OpenSnap()
        {
            System.Diagnostics.Debug.Assert(m_Camera.GetHandle() != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVOpenSnap(m_Camera.GetHandle(), snapCallback, this.Handle);
            USBCameraAPI.HV_VERIFY(status);
            if (USBCameraAPI.HV_SUCCESS(status))
            {
                m_bIsOpen = true;
            }
        }

        private void StartSnap()
        {
            System.Diagnostics.Debug.Assert(m_Camera.GetHandle() != IntPtr.Zero);
            IntPtr[] pBuffers = new IntPtr[1];
            pBuffers[0] = m_Camera.GetRawBuffer();
            HVSTATUS status = USBCameraAPI.HVStartSnap(m_Camera.GetHandle(), pBuffers, 1);
            USBCameraAPI.HV_VERIFY(status);
            if (USBCameraAPI.HV_SUCCESS(status))
            {
                m_bIsSnap = true;
            }
        }

        private void StopSnap()
        {
            System.Diagnostics.Debug.Assert(m_Camera.GetHandle() != IntPtr.Zero);
            if (m_bIsSnap)
            {
                HVSTATUS status = USBCameraAPI.HVStopSnap(m_Camera.GetHandle());
                USBCameraAPI.HV_VERIFY(status);
                if (USBCameraAPI.HV_SUCCESS(status))
                {
                    m_bIsSnap = false;
                }
            }
        }

        private void CloseSnap()
        {
            System.Diagnostics.Debug.Assert(m_Camera.GetHandle() != IntPtr.Zero);
            StopSnap();
            HVSTATUS status = USBCameraAPI.HVCloseSnap(m_Camera.GetHandle());
            USBCameraAPI.HV_VERIFY(status);
            if (USBCameraAPI.HV_SUCCESS(status))
            {
                m_bIsOpen = false;
            }
        }

        private static bool SnapCallBack(ref HV_SNAP_INFO pInfo)
        {
            SMTDlg dlg = (SMTDlg)(Form.FromHandle(pInfo.pParam));
            if (dlg != null)
            {
                dlg.ShowImage();
            }
            return true;
        }

        private void ShowImage()
        {
            m_Camera.SaveImage();
            Graphics gc = ccdView.CreateGraphics();
            gc.DrawImage(m_Camera.GetCurrentBMP(), ccdView.ClientRectangle);
            m_src = new Image<Bgr, Byte>(m_Camera.GetCurrentBMP());
        }

#endregion


        private void SetGain_Scroll(object sender, EventArgs e)
        {
            gainUpDown.Value = gainTrackBar.Value;
            m_Camera.SetGain(gainTrackBar.Value);
        }

        private void SetExposure_Scroll(object sender, EventArgs e)
        {
            exposureUpDown.Value = exposureTrackBar.Value;
            m_Camera.SetExposureTime(exposureTrackBar.Value);
        }

        private void SetGainUpDown(object sender, EventArgs e)
        {
            gainTrackBar.Value = System.Convert.ToInt32(gainUpDown.Value);
            m_Camera.SetGain(gainTrackBar.Value);
        }

        private void SetExposureUpDown(object sender, EventArgs e)
        {
            exposureTrackBar.Value = System.Convert.ToInt32(exposureUpDown.Value);
            m_Camera.SetExposureTime(exposureTrackBar.Value);
        }

        private void stageXDownBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(stageStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(0, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 0, m_dmc3400ACard.TransDistanceToPulse(0, stepSize), -1, 0);
        }

        private void stageXUpBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(stageStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(0, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 0, m_dmc3400ACard.TransDistanceToPulse(0, stepSize), 1, 0);
        }

        private void stageYDownBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(stageStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(1, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 1, m_dmc3400ACard.TransDistanceToPulse(1, stepSize), -1, 0);
        }

        private void stageYUpBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(stageStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(1, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 1, m_dmc3400ACard.TransDistanceToPulse(1, stepSize), 1, 0);
        }

        private void CCDXDownBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(CCDStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(2, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 2, m_dmc3400ACard.TransDistanceToPulse(2, stepSize), -1, 0);
        }

        private void CCDXUpBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(CCDStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(2, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 2, m_dmc3400ACard.TransDistanceToPulse(2, stepSize), 1, 0);
        }

        private void CCDZDownBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(CCDStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(3, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 3, m_dmc3400ACard.TransDistanceToPulse(3, stepSize), -1, 0);
        }

        private void CCDZUpBtn_Click(object sender, EventArgs e)
        {
            int stepSize = Convert.ToInt32(CCDStepEdit.Text);
            m_dmc3400ACard.SetMotionParam(3, m_dmc3400ACard.m_stageXAxisParam);
            m_dmc3400ACard.DMC3400_Move(m_dmc3400ACard.m_cardNo, 3, m_dmc3400ACard.TransDistanceToPulse(3, stepSize), 1, 0);
        }

        private void inhaleOpenBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_write_outbit(m_dmc3400ACard.m_cardNo, 0, 1);
        }

        private void inhaleCloseBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_write_outbit(m_dmc3400ACard.m_cardNo, 0, 0);
        }

        private void showCrossBtn_Click(object sender, EventArgs e)
        {

        }

        private void showImageScaleBtn_Click(object sender, EventArgs e)
        {

        }

        private void measureDisBtn_Click(object sender, EventArgs e)
        {

        }

        private void drawLineBtn_Click(object sender, EventArgs e)
        {

        }

        private void drawRectBtn_Click(object sender, EventArgs e)
        {

        }

        private void drawCircleBtn_Click(object sender, EventArgs e)
        {
            
        }

        private void stageStepEdit_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 0x20) e.KeyChar = (char)0;  //禁止空格键  
            if ((e.KeyChar == 0x2D) && (((TextBox)sender).Text.Length == 0)) return;   //处理负数  
            if (e.KeyChar > 0x20)
            {
                try
                {
                    double.Parse(((TextBox)sender).Text + e.KeyChar.ToString());
                }
                catch
                {
                    e.KeyChar = (char)0;   //处理非法字符  
                }
            }  
        }

        private void CCDStepEdit_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 0x20) e.KeyChar = (char)0;  //禁止空格键  
            if ((e.KeyChar == 0x2D) && (((TextBox)sender).Text.Length == 0)) return;   //处理负数  
            if (e.KeyChar > 0x20)
            {
                try
                {
                    double.Parse(((TextBox)sender).Text + e.KeyChar.ToString());
                }
                catch
                {
                    e.KeyChar = (char)0;   //处理非法字符  
                }
            }  
        }
    }
}
