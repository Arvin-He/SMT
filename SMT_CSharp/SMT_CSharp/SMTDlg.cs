using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using USBCamera;
using System.Drawing.Imaging;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.Util;

using USBCameraAPI = USBCamera.API;
using USBDHCamera;

namespace SMT_CSharp
{
    public partial class SMTDlg : Form
    {
#region public method

        public SMTDlg()
        {
            InitializeComponent();
            m_Camera.Initialize();
//             m_capture = null;
//             m_capture = new Capture(0);
//             m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FPS, 15);
//             m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT, 480);
//             m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH, 640);
        }
        ~SMTDlg()
        {
            CloseSnap();
            m_Camera.Release();
        }
#endregion

#region private method
//         private void ProcessFrame(object sender, EventArgs arg)
//         {
//             //capture to a Image variable so we can use it for writing to the VideoWriter
//             Image<Bgr, Byte> frame = m_capture.RetrieveBgrFrame(); 
//             //if we wanted to compresse the image to a smaller size to save space on our video we could use
//             //frame.Resize(100,100, Emgu.CV.CvEnum.INTER.CV_INTER_LINEAR)
//             //But the VideoWriter must be set up with the correct size
//             if (m_videoWriter.Ptr != IntPtr.Zero)
//             {
//                 m_videoWriter.WriteFrame(frame); //If we are recording and videowriter is avaliable add the image to the videowriter 
//                 //Update frame number
//                 m_frameCount++;
//             }
//         }

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
                Bitmap saveImage = m_Camera.GetCurrentBMP();
                saveImage.Save(imageSaveDlg.FileName);
            }
        }
        private void SaveVideo_Click(object sender, EventArgs e)
        {
//             SaveFileDialog videoSaveDlg = new SaveFileDialog();
//             videoSaveDlg.InitialDirectory = "./data/videos";
//             videoSaveDlg.Filter = "avi file(*.avi)|*.avi;*.mp4;*.mpg";
//             if (videoSaveDlg.ShowDialog() == DialogResult.OK)
//             {
//                 if (m_capture != null)
//                 {
//                     if (m_capture.GrabProcessState == System.Threading.ThreadState.Running)
//                     {
//                         m_capture.Stop(); //Stop urrent capture if running 
//                     }
//                     m_capture.Dispose();//dispose of current capture
//                 }
//                 //保存视频
//                 try
//                 {
//                     m_capture.ImageGrabbed += ProcessFrame; //attach event call to process frames
//                     int frameWidth = (int)m_capture.GetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH);
//                     int frameHeight = (int)m_capture.GetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT);
//                     int frameRate = (int)m_capture.GetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FPS);
//                     m_videoWriter = new VideoWriter(@videoSaveDlg.FileName, -1, (int)frameRate, frameWidth, frameHeight, true);
//                     m_capture.Start();
//                 }
//                 catch (NullReferenceException excpt)
//                 {
//                     MessageBox.Show(excpt.Message);
//                 }
//             }
        }
        private void StopSaveVideo_Click(object sender, EventArgs e)
        {
//             if (m_capture != null)
//             {
//                 if (m_capture.GrabProcessState == System.Threading.ThreadState.Running)
//                 {
//                     m_capture.Stop();
//                 }
//                 m_capture.Dispose();
//             }
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
        }
#endregion

#region variable
        USBDHCamera.USBDHCamera m_Camera = new USBDHCamera.USBDHCamera();
        private HV_SNAP_PROC snapCallback = new HV_SNAP_PROC(SnapCallBack);
        private bool m_bIsSnap = false;
        private bool m_bIsOpen = false;
        //private Capture m_capture;
        //private VideoWriter m_videoWriter;
        //private int m_frameCount = 0;
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



    }
}
