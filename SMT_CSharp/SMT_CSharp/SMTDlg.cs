using System;
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
using System.IO;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.Util;
using USBCamera;
using USBCameraAPI = USBCamera.API;
using DH3151UC;
using DMC3400A;
using csLTDMC;
using INIConfig;
using SMT_CSharp;
using SMT_CSharp.Properties;

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
        System.Timers.Timer timer = new System.Timers.Timer();
        Image m_greenImg = Image.FromFile(@"../../Properties/res/bmp/green.bmp");  // 相对于exe的路径
        Image m_redImg = Image.FromFile(@"../../Properties/res/bmp/red.bmp");
#endregion

#region public method
        public SMTDlg()
        {
            InitializeComponent();
        }

        ~SMTDlg()
        {
            CloseSnap();
            m_Camera.Release();
            timer.Stop();
        }
#endregion

#region private method
        private void SMTDlg_Load(object sender, EventArgs e)
        {
            m_Camera.Initialize();
            m_dmc3400ACard.InitDMC3400ACard();
            InitTimer();
            timer.Start();
        }

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
            System.IO.DirectoryInfo topDir = System.IO.Directory.GetParent(System.Environment.CurrentDirectory);
            //继续获取上级的上级的上级的目录。
            string pathto = topDir.Parent.FullName;
            string configFilePath = Path.Combine(pathto, @"config\ccdConfig.ini");
            INIFileHelper inifile = new INIFileHelper(configFilePath);         
            inifile.WriteIniString("section1", "CCD增益", Convert.ToString(gainUpDown.Value));
            inifile.WriteIniString("section2", "CCD曝光度", Convert.ToString(exposureUpDown.Value));
        }

        private void LoadCCDParam_Click(object sender, EventArgs e)
        {
            System.IO.DirectoryInfo topDir = System.IO.Directory.GetParent(System.Environment.CurrentDirectory);
            string pathto = topDir.Parent.FullName;
            string configFilePath = Path.Combine(pathto, @"config\ccdConfig.ini");
            StringBuilder strCCDGain = new StringBuilder(32);
            StringBuilder strCCDExposure = new StringBuilder(32);
            INIFileHelper inifile = new INIFileHelper(configFilePath);
            inifile.GetIniString("section1", "CCD增益", "", strCCDGain, strCCDGain.Capacity);
            inifile.GetIniString("section2", "CCD曝光度", "", strCCDExposure, strCCDExposure.Capacity);
            string gain = strCCDGain.ToString();
            string exposure = strCCDExposure.ToString();
            gainUpDown.Value = Convert.ToDecimal(gain);
            gainTrackBar.Value = Convert.ToInt32(gain);
            exposureUpDown.Value = Convert.ToDecimal(exposure);
            exposureTrackBar.Value = Convert.ToInt32(exposure);
        }
        
        private void UpdateUI()
        {
            if (m_bIsOpen)
            {
                stopCCDBtn.Enabled = true;
                closeCCDBtn.Enabled = true;
                savePicBtn.Enabled = true;
                saveVideoBtn.Enabled = true;
                closeVideoBtn.Enabled = true;
            } 
            else
            {
                stopCCDBtn.Enabled = false;
                closeCCDBtn.Enabled = false;
                savePicBtn.Enabled = false;
                saveVideoBtn.Enabled = false;
                closeVideoBtn.Enabled = false;
            }
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
            //m_src = new Image<Bgr, Byte>(m_Camera.GetCurrentBMP());
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

        private void stageStepEdit_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 0x20) e.KeyChar = (char)0;  //禁止空格键  
            if ((e.KeyChar == 0x2D) && (((TextBox)sender).Text.Length == 0)) return;   //处理负数  
            if (e.KeyChar > 0x20)
            {
                try { double.Parse(((TextBox)sender).Text + e.KeyChar.ToString()); }
                catch { e.KeyChar = (char)0; }  //处理非法字符  
            }  
        }

        private void CCDStepEdit_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 0x20) e.KeyChar = (char)0;  //禁止空格键  
            if ((e.KeyChar == 0x2D) && (((TextBox)sender).Text.Length == 0)) return;   //处理负数  
            if (e.KeyChar > 0x20)
            {
                try { double.Parse(((TextBox)sender).Text + e.KeyChar.ToString()); }
                catch { e.KeyChar = (char)0; }  //处理非法字符  
            }  
        }

        private void setZeroStageXBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_set_position(m_dmc3400ACard.m_cardNo, 0, 0);
        }

        private void setZeroStageYBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_set_position(m_dmc3400ACard.m_cardNo, 1, 0);
        }

        private void setZeroCCDXBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_set_position(m_dmc3400ACard.m_cardNo, 2, 0);
        }

        private void setZeroCCDZBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_set_position(m_dmc3400ACard.m_cardNo, 3, 0);
        }

        private void stopStageXAxisBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_stop(m_dmc3400ACard.m_cardNo, 0, 0);
        }

        private void stopStageYAxisBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_stop(m_dmc3400ACard.m_cardNo, 1, 0);
        }

        private void stopCCDXAxisBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_stop(m_dmc3400ACard.m_cardNo, 2, 0);
        }

        private void stopCCDZAxisBtn_Click(object sender, EventArgs e)
        {
            LTDMC.dmc_stop(m_dmc3400ACard.m_cardNo, 3, 0);
        }

        private void InitTimer()
        {
            // 调用本方法开始用计算器  
            timer.Interval = 1000;//设置时钟周期为1秒（1000毫秒）  
            timer.Enabled = true;
            timer.AutoReset = true;
            timer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Elapsed);       
        }

        private void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            UpdateDMC3400Status();
            UpdateDMC3400Pulse();
            UpdateDMC3400Distance();
        }

        // 更新状态位
        private void UpdateDMC3400Status()
        {
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 0) & 0x02))
            { el1.Image = m_greenImg.Clone() as Image; }
            else
            { el1.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 1) & 0x02))
            { el2.Image = m_greenImg.Clone() as Image; }
            else
            { el2.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 2) & 0x02))
            { el3.Image = m_greenImg.Clone() as Image; }
            else
            { el3.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 3) & 0x02))
            { el4.Image = m_greenImg.Clone() as Image; }
            else
            { el4.Image = m_redImg.Clone() as Image; }

            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 0) & 0x04))
            { el5.Image = m_greenImg.Clone() as Image; }
            else
            { el5.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 1) & 0x04))
            { el6.Image = m_greenImg.Clone() as Image; }
            else
            { el6.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 2) & 0x04))
            { el7.Image = m_greenImg.Clone() as Image; }
            else
            { el7.Image = m_redImg.Clone() as Image; }
            if (Convert.ToBoolean(LTDMC.dmc_axis_io_status(m_dmc3400ACard.m_cardNo, 3) & 0x04))
            { el8.Image = m_greenImg.Clone() as Image; }
            else
            { el8.Image = m_redImg.Clone() as Image; }  
        }

        // 更新脉冲数
        private void UpdateDMC3400Pulse()
        {
            stageXPulseEdit.Text = Convert.ToString(LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 0));
            stageYPulseEdit.Text = Convert.ToString(LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 1));
            CCDXPulseEdit.Text = Convert.ToString(LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 2));
            CCDZPulseEdit.Text = Convert.ToString(LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 3));
        }
        // 更新距离
        private void UpdateDMC3400Distance()
        {
            stageXDisEdit.Text = Convert.ToString(m_dmc3400ACard.TransPulseToDistance(0, LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 0)));
            stageYDisEdit.Text = Convert.ToString(m_dmc3400ACard.TransPulseToDistance(1, LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 1)));
            CCDXDisEdit.Text = Convert.ToString(m_dmc3400ACard.TransPulseToDistance(2, LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 2)));
            CCDZDisEdit.Text = Convert.ToString(m_dmc3400ACard.TransPulseToDistance(3, LTDMC.dmc_get_position(m_dmc3400ACard.m_cardNo, 3)));
        }

        private void ccdView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                m_startPt = new Point(e.X, e.Y);
                m_endPt = new Point(e.X, e.Y);
            }
            else if (e.Button == MouseButtons.Right)
            {
                Graphics g = ccdView.CreateGraphics(); 
                g.Clear(Color.DarkGray);
            }
        }

        private void ccdView_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (m_bDrawLine || m_bDrawRect || m_bDrawCircle)
                {
                    m_endPt.X = e.X;
                    m_endPt.Y = e.Y;
                    ccdView.Refresh();
                }
            }
        }

        private void ccdView_MouseUp(object sender, MouseEventArgs e)
        {
            m_bDrawLine = false;
            m_bDrawRect = false;
            m_bDrawCircle = false;
        }

        private void ccdView_Paint(object sender, PaintEventArgs e)
        {
            if (m_startPt.Equals(m_endPt)) return;
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;//消除锯齿
            if (m_bDrawLine && (!m_bMeasureDis))
            {
                e.Graphics.DrawLine(System.Drawing.Pens.Blue, m_startPt, m_endPt);
            }
            else if (m_bDrawLine && m_bMeasureDis)
            {
                Point diff = new Point(m_endPt.X - m_startPt.X, m_endPt.Y - m_endPt.Y);
                double dis = Math.Sqrt(diff.X * diff.X + diff.Y * diff.Y);
                String str = string.Format("两点距离:{0:f}", dis);
                Font font = new Font("宋体", 8);
                SolidBrush sbrush = new SolidBrush(Color.Green);
                e.Graphics.DrawString(str, font, sbrush, m_endPt);
                e.Graphics.DrawLine(System.Drawing.Pens.Green, m_startPt, m_endPt);
            }
            else if (m_bDrawRect)
            {
                if (m_endPt.X - m_startPt.X < 0 || m_endPt.Y - m_startPt.Y < 0) return;
                e.Graphics.DrawRectangle(System.Drawing.Pens.Blue, m_startPt.X, m_startPt.Y, m_endPt.X - m_startPt.X, m_endPt.Y - m_startPt.Y);
            }
            else if (m_bDrawCircle)
            {
                Point diff = new Point(m_endPt.X - m_startPt.X, m_endPt.Y - m_endPt.Y);
                int radius = (int)Math.Sqrt(diff.X * diff.X + diff.Y * diff.Y);
                e.Graphics.DrawEllipse(System.Drawing.Pens.Blue, m_startPt.X, m_startPt.Y, radius, radius);
            }
        }

        private void showCrossBtn_Click(object sender, EventArgs e)
        {
            Graphics g = ccdView.CreateGraphics();
            Point pt1 = new Point(ccdView.Size.Width/2-10, ccdView.Size.Height/2);
            Point pt2 = new Point(ccdView.Size.Width/2+10, ccdView.Size.Height/2);
            Point pt3 = new Point(ccdView.Size.Width/2, ccdView.Size.Height/2-10);
            Point pt4 = new Point(ccdView.Size.Width/2, ccdView.Size.Height/2+10);
            if (m_bDrawCross)
            {
                g.DrawLine(System.Drawing.Pens.Blue, pt1, pt2);
                g.DrawLine(System.Drawing.Pens.Blue, pt3, pt4);
                m_bDrawCross = false;
            } 
            else
            {
                g.Clear(Color.DarkGray);
                m_bDrawCross = true;
            }
        }

        private void showImageScaleBtn_Click(object sender, EventArgs e)
        {
            Graphics g = ccdView.CreateGraphics();
            if (m_bDrawScale)
            {
                String str = "比例尺";
                Font font = new Font("宋体", 8);
                SolidBrush sbrush = new SolidBrush(Color.Green);
                g.DrawString(str, font, sbrush, new Point(ccdView.Size.Width - 65, ccdView.Size.Height-22));
                g.FillRectangle(new SolidBrush(Color.Green), ccdView.Size.Width - 70, ccdView.Size.Height-10, 50, 5);
                m_bDrawScale = false;
            } 
            else
            {
                g.Clear(Color.DarkGray);
                m_bDrawScale = true;
            }
        }

        private void measureDisBtn_Click(object sender, EventArgs e)
        {
            if (m_bMeasureDis)
            {
                m_bMeasureDis = false;
            } 
            else
            {
                m_bMeasureDis = true;
            }
        }

        private void drawLineBtn_Click(object sender, EventArgs e)
        {
            m_bDrawLine = true;
        }

        private void drawRectBtn_Click(object sender, EventArgs e)
        {
            m_bDrawRect = true;
        }

        private void drawCircleBtn_Click(object sender, EventArgs e)
        {
            m_bDrawCircle = true;
        }

        #region variable
        private bool m_bDrawCross = true;
        private bool m_bDrawScale = true;
        private bool m_bMeasureDis = true;
        private bool m_bDrawLine = false;
        private bool m_bDrawRect = false;
        private bool m_bDrawCircle = false;
        private Point m_startPt;
        private Point m_endPt;
        #endregion       
    }
}
