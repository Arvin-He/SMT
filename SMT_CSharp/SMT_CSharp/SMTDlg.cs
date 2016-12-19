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
//using Emgu.CV.Capture;

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
                Bitmap saveImage = m_Camera.GetCurrentBMP();
                saveImage.Save(imageSaveDlg.FileName);
            }
        }
        private void SaveVideo_Click(object sender, EventArgs e)
        {
            SaveFileDialog videoSaveDlg = new SaveFileDialog();
            videoSaveDlg.InitialDirectory = "./data/videos";
            videoSaveDlg.Filter = "avi file(*.avi)|*.avi;*.mp4;*.mpg";
            if (videoSaveDlg.ShowDialog() == DialogResult.OK)
            {
                //Bitmap saveImage = m_Camera.GetCurrentBMP();
                //saveImage.Save(imageSaveDlg.FileName);
                //保存视频
                try
                {
                    m_capture = null;
                    m_capture = new Capture(0);
                    m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FPS, 30);
                    m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT, 480);
                    m_capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH, 640);
                    //Time_Label.Text = "Time: ";
                    //Codec_lbl.Text = "Codec: ";
                    //Frame_lbl.Text = "Frame: ";
                    //webcam_frm_cnt = 0;
                    //cam = 1;
                    //Video_seek.Value = 0;
                    //Application.Idle += ProcessFrame;
                    //button1.Text = "Stop";
                    //comboBox1.Enabled = false;
                }
                catch (NullReferenceException excpt)
                {
                    MessageBox.Show(excpt.Message);
                }
            }
        }
        private void StopSaveVideo_Click(object sender, EventArgs e)
        {
            if (m_capture != null)
            {
                if (m_capture.GrabProcessState == System.Threading.ThreadState.Running)
                {
                    m_capture.Stop();
                }
                m_capture.Dispose();
            }
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
            //Graphics gc = this.CreateGraphics();
            //gc.DrawImage(m_Camera.GetCurrentBMP(), this.ClientRectangle);
            Graphics gc = ccdView.CreateGraphics();
            gc.DrawImage(m_Camera.GetCurrentBMP(), ccdView.ClientRectangle);
        }
#endregion

#region variable
        USBDHCamera.USBDHCamera m_Camera = new USBDHCamera.USBDHCamera();
        private HV_SNAP_PROC snapCallback = new HV_SNAP_PROC(SnapCallBack);
        private bool m_bIsSnap = false;
        private bool m_bIsOpen = false;
        private Capture m_capture;
#endregion
        private void SetGain_Scroll(object sender, EventArgs e)
        {
            gainUpDown.Value = gainTrackBar.Value;
        }

        private void SetExposure_Scroll(object sender, EventArgs e)
        {
            exposureUpDown.Value = exposureTrackBar.Value;
        }

        private void SetGainUpDown(object sender, EventArgs e)
        {
            gainTrackBar.Value = System.Convert.ToInt32(gainUpDown.Value);
        }

        private void SetExposureUpDown(object sender, EventArgs e)
        {
            exposureTrackBar.Value = System.Convert.ToInt32(exposureUpDown.Value);
        }



    }
}
