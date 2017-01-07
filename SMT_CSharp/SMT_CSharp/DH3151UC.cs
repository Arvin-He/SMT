using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using USBCamera;
using System.Runtime.InteropServices;

using USBCameraAPI = USBCamera.API;
using System.Diagnostics;

namespace DH3151UC
{
    public interface ICamera
    {
        void BeginDevice();                         ///< 打开设备
        void GetCameraType();                       ///< 获取摄像机类型
        void SetResolution();                       ///< 设置分辨率
        void SetSnapMode();                         ///< 设置采集模式
        void SetGain(int gainValue);                ///< 设置增益
        void SetADC();                              ///< 设置数字增益
        void SetOutPutWindow();                     ///< 设置输出窗口
        void SetBlanking();                         ///< 设置消隐控制
        void SetSnapSpeed();                        ///< 设置采集速度
        void SetExposureTime(int exposureTime);     ///< 设置曝光时间
        void EndDevice();                           ///< 关闭设备
        void SaveImage();                           ///< 显示图像
    }

    public class USBDHCamera : ICamera
    {
#region variable
        protected IntPtr m_pHandle = IntPtr.Zero;           ///< 句柄
        protected string m_strCameraType = string.Empty;    ///< 相机类型
        protected byte[] m_RawBuffer;                       ///< Raw图像缓存
        protected IntPtr m_pRawBuffer;                      ///< Raw图像缓存指针
        protected byte[] m_ImageBuffer;                     ///< RGB图像缓存
        protected byte[] m_LutR = new byte[256];            ///< 颜色查询表R分量
        protected byte[] m_LutG = new byte[256];            ///< 颜色查询表G分量
        protected byte[] m_LutB = new byte[256];            ///< 颜色查询表B分量
        protected Bitmap m_bmpCurrent;                      ///< 当前位图
#endregion

#region constant
        protected const int m_kCameraNums = 1;                                                              ///< 相机个数
        protected const HV_RESOLUTION m_kResolotion = HV_RESOLUTION.RES_MODE0;                              ///< 分辨率
        protected const HV_SNAP_MODE m_kSnapMode = HV_SNAP_MODE.CONTINUATION;                               ///< 采集模式
        protected int m_kGain = 9;                                                                          ///< 增益
        protected const int m_kADCLevel = (int)HV_ADC_LEVEL.ADC_LEVEL2;                                     ///< 增益级别
        protected Rectangle m_OutPutWindow = new Rectangle(0, 0, 640, 480);                                 ///< 输出窗口
        protected const int m_kHBlanking = 0;                                                               ///< 水平消隐
        protected const int m_kVBlanking = 0;                                                               ///< 垂直消隐
        protected const HV_SNAP_SPEED m_kSnapSpeed = HV_SNAP_SPEED.NORMAL_SPEED;                            ///< 采集速度
        protected const int m_kUpperET = 60;                                                                ///< 曝光时间分子
        protected int m_kLowerET = 1000;                                                                    ///< 曝光时间分母
        protected const double m_kZeorInDouble = 0.000000001;                                               ///< double类型的0
        protected const HV_BAYER_CONVERT_TYPE m_kConvertType = HV_BAYER_CONVERT_TYPE.BAYER2RGB_NEIGHBOUR;   ///< 转换类型
        protected const HV_BAYER_LAYOUT m_kBayerType = HV_BAYER_LAYOUT.BAYER_GR;                            ///< Bayer类型
        protected const ImageLockMode m_kLockMode = ImageLockMode.WriteOnly;                                ///< 图像锁定模式
        protected const PixelFormat m_kBMPFormat = PixelFormat.Format24bppRgb;                              ///< 图像像素格式
#endregion

#region public method
        public void Initialize()
        {
            InitializeCamera();
            InitializeData();
        }

        public void Release()
        {
            ReleaseData();
            EndDevice();
        }

        public void BeginDevice()
        {
            if (m_pHandle == IntPtr.Zero)
            {
                HVSTATUS status = USBCameraAPI.BeginHVDevice(m_kCameraNums, ref m_pHandle);
                USBCameraAPI.HV_VERIFY(status);
            }
        }

        public void GetCameraType()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            IntPtr buffer = new IntPtr();
            int size = sizeof(HVTYPE);
            StringBuilder str = new StringBuilder();
            buffer = Marshal.AllocHGlobal(size);
            HVSTATUS status = USBCameraAPI.HVGetDeviceInfo(m_pHandle, HV_DEVICE_INFO.DESC_DEVICE_TYPE, buffer, ref size);
            USBCameraAPI.HV_VERIFY(status);
            int[] type = new int[size / 4];
            Marshal.Copy(buffer, type, 0, size / 4);
            for (int i = 0; i < size / 4; i++)
            {
                str.Append(((HVTYPE)type[i]).ToString().Substring(0, 8));
            }
            m_strCameraType = str.ToString();
            str.Remove(0, str.Length);
            Marshal.FreeHGlobal(buffer);
        }

        public void SetResolution()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVSetResolution(m_pHandle, m_kResolotion);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetSnapMode()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVSetSnapMode(m_pHandle, m_kSnapMode);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetGain(int gainValue)
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            m_kGain = gainValue;
            HVSTATUS status = HVSTATUS.STATUS_OK;
            for (int i = 0; i < 4; i++)
            {
                status = USBCameraAPI.HVAGCControl(m_pHandle, (byte)(HV_CHANNEL.RED_CHANNEL + i), m_kGain);
                USBCameraAPI.HV_VERIFY(status);
            }
        }

        public void SetADC()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVADCControl(m_pHandle, (byte)HV_ADC_CONTROL.ADC_BITS, m_kADCLevel);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetOutPutWindow()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            IntPtr buffer = new IntPtr();
            int size = 0;
            HVSTATUS status = USBCameraAPI.HVGetDeviceInfo(m_pHandle, HV_DEVICE_INFO.DESC_RESOLUTION, buffer, ref size);
            buffer = Marshal.AllocHGlobal(size);
            status = USBCameraAPI.HVGetDeviceInfo(m_pHandle, HV_DEVICE_INFO.DESC_RESOLUTION, buffer, ref size);
            USBCameraAPI.HV_VERIFY(status);
            int[] type = new int[64];
            Marshal.Copy(buffer, type, 0, 64);           
            Marshal.FreeHGlobal(buffer);

            m_OutPutWindow.Width = type[(int)m_kResolotion*2];
            m_OutPutWindow.Height = type[(int)m_kResolotion*2+1];

            status = USBCameraAPI.HVSetOutputWindow(m_pHandle, m_OutPutWindow.X, m_OutPutWindow.Y, m_OutPutWindow.Width, m_OutPutWindow.Height);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetBlanking()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVSetBlanking(m_pHandle, m_kHBlanking, m_kVBlanking);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetSnapSpeed()
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            HVSTATUS status = USBCameraAPI.HVSetSnapSpeed(m_pHandle, m_kSnapSpeed);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void SetExposureTime(int exposureTime)
        {
            System.Diagnostics.Debug.Assert(m_pHandle != IntPtr.Zero);
            m_kLowerET = exposureTime;
            HVSTATUS status = SetExposureTime(m_OutPutWindow.Width, m_kUpperET, m_kLowerET, 
                                                m_kHBlanking, m_kSnapSpeed, m_kResolotion);
            USBCameraAPI.HV_VERIFY(status);
        }

        public void EndDevice()
        {
            if (m_pHandle != IntPtr.Zero)
            {
                HVSTATUS status = USBCameraAPI.EndHVDevice(m_pHandle);
                USBCameraAPI.HV_VERIFY(status);
            }
        }

        public void SaveImage()
        {
            Marshal.Copy(m_pRawBuffer, m_RawBuffer, 0, m_OutPutWindow.Width * m_OutPutWindow.Height);
            USBCameraAPI.ConvertBayer2Rgb(m_ImageBuffer, m_RawBuffer, m_OutPutWindow.Width, m_OutPutWindow.Height,
                                        m_kConvertType, m_LutR, m_LutG, m_LutB, false, m_kBayerType);

            BitmapData bmpData = m_bmpCurrent.LockBits(new Rectangle(m_OutPutWindow.X, m_OutPutWindow.Y,
                                                                            m_OutPutWindow.Width, m_OutPutWindow.Height),
                                                            m_kLockMode, m_kBMPFormat);
            Marshal.Copy(m_ImageBuffer, 0, bmpData.Scan0, m_OutPutWindow.Width * m_OutPutWindow.Height * 3);
            m_bmpCurrent.UnlockBits(bmpData);
        }

        public bool IsHV300()
        {
            if ((m_strCameraType == "HV3000UCTYPE") || (m_strCameraType == "HV3102UCTYPE") ||
                (m_strCameraType == "HV3103UCTYPE") || (m_strCameraType == "HV3150UCTYPE") ||
                (m_strCameraType == "HV3151UCTYPE")) 
            {
                return true;
            } 
            else 
            {
                return false;
            }
        }        

        public IntPtr GetRawBuffer()
        {
            return m_pRawBuffer;
        }

        public IntPtr GetHandle()
        {
            return m_pHandle;
        }

        public Bitmap GetCurrentBMP()
        {
            return m_bmpCurrent;
        }
#endregion

#region protected method
        protected HVSTATUS SetExposureTime(int nWindWidth, int nUpper, int nLower, int nHBlanking,
                                            HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution)
        {
            double clockFreq = 0.0;
            int tB = nHBlanking;
            int outPut = nWindWidth;
            double exposure = 0.0;
            Debug.Assert(nLower != 0);
            double temp = (double)nUpper / (double)nLower;
            double tInt = (temp > m_kZeorInDouble) ? temp : m_kZeorInDouble;

            if (IsHV300())
            {
                clockFreq = (SnapSpeed == HV_SNAP_SPEED.HIGH_SPEED) ? 24000000 : 12000000;
                tB += 9;
                tB -= 19;
                if (tB <= 0) { tB = 0; }

                if ((outPut + 244.0 + tB) > 552) { exposure = (tInt * clockFreq + 180.0) / ((double)outPut + 244.0 + tB);} 
                else { exposure = ((tInt * clockFreq) + 180.0) / 552; }

                if ((exposure - (int)exposure) > 0.5) { exposure += 1.0;}

                if (exposure <= 0) { exposure = 1;} 
                else if (exposure > 16383) { exposure = 16383;}
            }
            return USBCameraAPI.HVAECControl(m_pHandle, (byte)HV_AEC_CONTROL.AEC_EXPOSURE_TIME, (int)exposure);
        }

        protected void InitializeCamera()
        {
            BeginDevice();
            if (m_pHandle != IntPtr.Zero) {
                GetCameraType();
                SetResolution();
                SetSnapMode();
                SetGain(m_kGain);
                SetADC();
                SetOutPutWindow();
                SetBlanking();
                SetSnapSpeed();
                SetExposureTime(m_kLowerET);
            } 
            else 
            {
                return;
            }
        }

        protected void InitializeData()
        {
            m_RawBuffer = new byte[m_OutPutWindow.Width * m_OutPutWindow.Height];
            m_ImageBuffer = new byte[m_OutPutWindow.Width * m_OutPutWindow.Height * 3];
            for (int i = 0; i < 256; i++)
            {
                m_LutR[i] = (byte)i;
                m_LutG[i] = (byte)i;
                m_LutB[i] = (byte)i;
            }
            m_bmpCurrent = new Bitmap(m_OutPutWindow.Width, m_OutPutWindow.Height, m_kBMPFormat);
            m_pRawBuffer = Marshal.AllocHGlobal(m_OutPutWindow.Width * m_OutPutWindow.Height);
        }

        private void ReleaseData()
        {
            Marshal.FreeHGlobal(m_pRawBuffer);
        }
#endregion
    }
}
