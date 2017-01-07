using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Windows.Forms;

namespace INIConfig
{
    class INIFileHelper
    {
        private string strIniFilePath = @"../../Config/ccdConfig.ini";

        // 返回0表示失败，非0为成功  
        [DllImport("kernel32", CharSet = CharSet.Auto)]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        // 返回取得字符串缓冲区的长度  
        [DllImport("kernel32", CharSet = CharSet.Auto)]
        private static extern long GetPrivateProfileString(string section, string key, string strDefault, StringBuilder retVal, int size, string filePath);
        /*
         [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
         public static extern int GetPrivateProfileInt(string section, string key, int nDefault, string filePath);  
        */

        public INIFileHelper()  
        {
        }  

        public INIFileHelper(string strIniFilePath)
        {
            if (File.Exists(strIniFilePath))
            {
                this.strIniFilePath = strIniFilePath;
            } 
            else
            {
                MessageBox.Show("文件不存在或文件路径错误!");
            }
        }
       
        /// <summary>
        /// 写入
        /// </summary>
        /// <param name="section">写入section</param>
        /// <param name="key">写入key</param>
        /// <param name="value">写入value</param>
        public void  WriteIniString(string section, string key, string value)  
        {
            try
            {
                WritePrivateProfileString(section, key, value, strIniFilePath);
            }
            catch
            {
                throw new Exception("配置文件不存在或权限不足导致无法写入");
            }         
        }

        /// <summary>  
        /// 获取ini配置文件中的字符串  
        /// </summary>  
        /// <param name="section">节名</param>  
        /// <param name="key">键名</param>  
        /// <param name="strDefault">默认值</param>  
        /// <param name="retVal">结果缓冲区</param>  
        /// <param name="size">结果缓冲区大小</param>  
        /// <returns>返回值</returns>  
        public string  GetIniString(string section, string key, string strDefault, StringBuilder retVal, int size)
        {
            long liRet = GetPrivateProfileString(section, key, strDefault, retVal, size, strIniFilePath);
            StringBuilder temp = new StringBuilder(1024);
            INIFileHelper.GetPrivateProfileString(section, key, strDefault, temp, 1024, strIniFilePath);
            return temp.ToString();
        }  
    }
}
