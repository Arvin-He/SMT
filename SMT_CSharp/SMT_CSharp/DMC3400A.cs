using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using csLTDMC;

namespace SMT_CSharp
{
    class DMC3400A
    {
        public ushort m_cardNo = 0;
        public const ushort m_axisCount = 4;
        public AxisParam m_stageXAxisParam = new AxisParam(0, 1000, 100, 0.02, 0.02, 0.2);
        public AxisParam m_stageYAxisParam = new AxisParam(0, 1000, 100, 0.02, 0.02, 0.2);
        public AxisParam m_ccdXAxisParam = new AxisParam(0, 1000, 100, 0.02, 0.02, 0.2);
        public AxisParam m_ccdZAxisParam = new AxisParam(0, 1000, 100, 0.02, 0.02, 0.2);
        public SoftLimitParam m_stageXSoftLimitParam = new SoftLimitParam(1000, 0);
        public SoftLimitParam m_stageYSoftLimitParam = new SoftLimitParam(1000, 0);
        public SoftLimitParam m_ccdXSoftLimitParam = new SoftLimitParam(1000, 0);
        public SoftLimitParam m_ccdZSoftLimitParam = new SoftLimitParam(1000, 0);

        public struct SoftLimitParam
        {
           public double nELUp;
           public double nELDown;
           public SoftLimitParam(double eLUp, double eLDown)
           {
               nELUp = eLUp;
               nELDown = eLDown;
           }
        }

        public struct AxisParam
        {
            public double nStartSpeed;
            public double nRunSpeed;
            public double nEndSpeed;
            public double nAccelerateTime;
            public double nDecelerateTime;
            public double nSDecelerateTime;
            public AxisParam(double startSpeed, double runSpeed, double endSpeed, 
                             double accTime, double decTime, double SDecTime)
            {
                nStartSpeed = startSpeed;
                nRunSpeed = runSpeed;
                nEndSpeed = endSpeed;
                nAccelerateTime = accTime;
                nDecelerateTime = decTime;
                nSDecelerateTime = SDecTime;
            }
        }

        public DMC3400A()
        {
            
        }

        ~DMC3400A()
        {

        }

        public bool InitDMC3400ACard()
        {
            short num = LTDMC.dmc_board_init();//获取卡数量
            if (num <= 0 || num > 8)
            {
                return false;
                //MessageBox.Show("初始卡失败!", "出错");
            }
            ushort _num = 0;
            ushort[] cardids = new ushort[8];
            uint[] cardtypes = new uint[8];
            short res = LTDMC.dmc_get_CardInfList(ref _num, cardtypes, cardids);
            if (res != 0)
            {
                return false;
                //MessageBox.Show("获取卡信息失败!");
            }
            m_cardNo = cardids[0];

	        for (ushort i=0; i<m_axisCount; i++)
	        {
		        // 设定脉冲模式及逻辑方向	
		        LTDMC.dmc_set_pulse_outmode(m_cardNo, i, 4);
		        // 设置 EL 限位信号
                //LTDMC.dmc_set_el_mode(m_cardNo, i, 1, 1, 0);
		        // 设置编码器的计数方式
                LTDMC.dmc_set_counter_inmode(m_cardNo, i, 0);
		        //设置指定轴的 EZ 信号
                //LTDMC.dmc_set_ez_mode(m_cardNo, i, 0);
	        }
	        return true;
        }

        public double TransPulseToDistance(int nAxisIndex, int nPulse)
        {
	        if (nAxisIndex == 0)
	        {
		        return nPulse;
	        }
	        else if (nAxisIndex == 1)
	        {
		        return nPulse;
	        }
	        else if (nAxisIndex == 2)
	        {
		        return 2*nPulse;
	        }
	        else if (nAxisIndex == 3)
	        {
		        return 2*nPulse;
	        }
	        else
	        {
		        return -1;
	        }
        }

        public int TransDistanceToPulse(int nAxisIndex, int nDistance)
        {
	        if (nAxisIndex == 0)
	        {
		        return nDistance;
	        }
	        else if (nAxisIndex == 1)
	        {
		        return nDistance;
	        }
	        else if (nAxisIndex == 2)
	        {
		        return nDistance/2;
	        }
	        else if (nAxisIndex == 3)
	        {
		        return nDistance/2;
	        }
	        else
	        {
		        return -1;
	        }
        }

        public void SetMotionParam(ushort nAxisIndex, AxisParam axisParam)
        {
		    // 设置单轴运动速度曲线
		    LTDMC.dmc_set_profile(m_cardNo, nAxisIndex, axisParam.nStartSpeed, axisParam.nRunSpeed,
                                  axisParam.nAccelerateTime, axisParam.nDecelerateTime, axisParam.nEndSpeed);
		    //设定S段时间
            LTDMC.dmc_set_s_profile(m_cardNo, nAxisIndex, 0, axisParam.nSDecelerateTime); //S 段时间，单位：s；范围：0~0.5 s
        }

        // 封装dmc运动函数:卡号, 轴号, 脉冲数, 运动方向, 运动模式
        public void DMC3400_Move(ushort nCardNo, ushort nAxisIndex, int nPulse, int nDirection, ushort nMoveMode)
        {
            if (LTDMC.dmc_check_done(m_cardNo, nAxisIndex) == 0) //已经在运动中
		        return; 
	        //点动(位置模式)
            LTDMC.dmc_pmove(m_cardNo, nAxisIndex, nPulse * nDirection, nMoveMode);  //最后的0表示相对运动
        }
    }
}
