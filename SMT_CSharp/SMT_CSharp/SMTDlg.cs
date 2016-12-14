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
        private void openCCDBtn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("hello");
            //OpenSnap();
            //StartSnap();
            //UpdateUI();
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
            Graphics gc = this.CreateGraphics();
            gc.DrawImage(m_Camera.GetCurrentBMP(), this.ClientRectangle);
        }
#endregion

#region variable
        USBDHCamera.USBDHCamera m_Camera = new USBDHCamera.USBDHCamera();
        private HV_SNAP_PROC snapCallback = new HV_SNAP_PROC(SnapCallBack);
        private bool m_bIsSnap = false;
        private bool m_bIsOpen = false;
#endregion
    }
}
