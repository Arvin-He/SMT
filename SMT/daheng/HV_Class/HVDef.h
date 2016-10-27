/*
 *-----------------------------------------------------------------------------
 *       Name				HVDef.h
 *       Purpose			Constant definition
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2009/05/13
 *		 Version			2.2.7.0
 *
 *       Description
 *		 This header file defines constant and external struct type
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVDEF)         //
#define HV_HVDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
typedef HANDLE HHV;


typedef enum tagHVTYPE { 	
	HV1300UCTYPE = 0,
	HV2000UCTYPE = 1,
	HV1301UCTYPE = 2,
	HV2001UCTYPE = 3,
	HV3000UCTYPE = 4,
	HV1300UMTYPE = 5,
	HV1302UCTYPE = 6,
	HV2002UCTYPE = 7,
	HV3102UCTYPE = 8,
	HV1302UMTYPE = 9,
	HV1300FCTYPE = 10,
	HV2000FCTYPE = 11,
	HV3100FCTYPE = 12,
	HV1300FMTYPE = 13,
	HV1303UCTYPE = 14,
	HV2003UCTYPE = 15,
	HV3103UCTYPE = 16,
	HV1303UMTYPE = 17,
	SV1300FMTYPE = 18,
 	SV1300FCTYPE = 19,
	SV1310FCTYPE = 20,
	SV1310FMTYPE = 21,
    SV1311FCTYPE = 22,
	SV1311FMTYPE = 23,
	SV400FCTYPE  = 24,
	SV400FMTYPE  = 25,
	DH1394FXTYPE = 26,
	SV1410FCTYPE = 27,
	SV1410FMTYPE = 28,
	SV1420FCTYPE = 29,
	SV1420FMTYPE = 30,
	SV2000FCTYPE = 31,
	SV2000FMTYPE = 32,
	SV1400FCTYPE = 33,
	SV1400FMTYPE = 34,
	HV1350UCTYPE = 35,
	HV2050UCTYPE = 36,
	HV3150UCTYPE = 37,
	HV1350UMTYPE = 38,
	HV1360UCTYPE = 39,
	HV2060UCTYPE = 40,
	HV3160UCTYPE = 41,
	HV1360UMTYPE = 42
} HVTYPE;


typedef enum tagHV_COLOR_CODE {
	COLOR_MONO8              = 0, ///< Y component has 8bit data
	COLOR_YUV8_4_1_1         = 1, ///< Y=U=V= 8bits, non compressed
	COLOR_YUV8_4_2_2         = 2, ///< Y=U=V=8bits, non compressed
	COLOR_YUV8_4_4_4         = 3, ///< Y=U=V=8bits, non compressed
	COLOR_RGB8               = 4, ///< RGB 8 format, each component has 8bit data
	COLOR_MONO16             = 5, ///< Y only, Y=16bits, non compressed(unsigned integer)
	COLOR_RGB16              = 6, ///< RGB 16 format, each component has 16bit data
	COLOR_SIGNED_MONO16      = 7, ///< Y only, Y=16bits, non compressed(signed integer)
	COLOR_SIGNED_RGB16       = 8, ///< R=G=B=16bits, non compressed(signed integer)
	COLOR_RAW8               = 9, 
	COLOR_RAW16              = 10,
	
	COLOR_MONO10             = 128,
	COLOR_MONO10_NORMALIZED  = 129,
	COLOR_MONO10_PACKED      = 130,
	COLOR_RAW10              = 131,
	COLOR_RAW10_NORMALIZED   = 132,
	COLOR_RAW10_PACKED       = 133,
	
	COLOR_MONO12             = 134,
	COLOR_MONO12_NORMALIZED  = 135,
	COLOR_MONO12_PACKED      = 136,
	COLOR_RAW12              = 137,
	COLOR_RAW12_NORMALIZED   = 138,
	COLOR_RAW12_PACKED       = 139
		
} HV_COLOR_CODE;

/// Color Filter ID
typedef enum tagHV_COLOR_FILTER {
	COLOR_FILTER_RG_GB  = 0, ///< RGB primary color filter (RG/GB)
	COLOR_FILTER_GB_RG  = 1, ///< RGB primary color filter (GB/RG)
	COLOR_FILTER_GR_BG  = 2, ///< RGB primary color filter (GR/BG)
	COLOR_FILTER_BG_GR  = 3 ///< RGB primary color filter (BG/GR)
} HV_COLOR_FILTER;


typedef enum tagHV_RESOLUTION { 
	RES_MODE0		= 0,
	RES_MODE1		= 1,
	RES_MODE2		= 2,
	RES_MODE3		= 3	
} HV_RESOLUTION;


typedef enum tagHV_SNAP_MODE { 
	CONTINUATION	= 0,
	TRIGGER			= 1,
	TRIGGER_EDGE	= 2,
	TRIGGER_LEVEL	= 3
} HV_SNAP_MODE;


typedef enum tagHV_POLARITY { 
	LOW	 = 0,
	HIGH = 1
} HV_POLARITY;


typedef enum tagHV_SNAP_SPEED { 
	NORMAL_SPEED	= 0,	
	HIGH_SPEED	    = 1		
} HV_SNAP_SPEED;


typedef enum tagHV_CHANNEL { 
	RED_CHANNEL			= 0x10,
	GREEN_CHANNEL1		= 0x11,
	GREEN_CHANNEL2		= 0x12,
	BLUE_CHANNEL		= 0x13	
} HV_CHANNEL;


typedef enum tagHV_ADC_CONTROL{ 
	ADC_BITS		                = 0,
	ADC_BLKLEVEL_CAL				= 1,
	ADC_BLKLEVEL_CAL_REDCHANNEL		= 0x10,
	ADC_BLKLEVEL_CAL_GREENCHANNEL1	= 0x11,
	ADC_BLKLEVEL_CAL_GREENCHANNEL2	= 0x12,
	ADC_BLKLEVEL_CAL_BLUECHANNEL	= 0x13
} HV_ADC_CONTROL;


typedef enum tagHV_ADC_LEVEL{ 
	ADC_LEVEL0		= 0,
	ADC_LEVEL1		= 1,
	ADC_LEVEL2		= 2,
	ADC_LEVEL3		= 3		
} HV_ADC_LEVEL;


typedef enum tagHV_AEC_CONTROL{ 
	AEC_EXPOSURE_TIME		= 1	
} HV_AEC_CONTROL;



typedef enum tagHV_MIRROR_DIRECTION { 
	VERT_DIR	= 1	
} HV_MIRROR_DIRECTION;


typedef enum tagHV_DEVICE_INFO { 
	DESC_DEVICE_TYPE			 = 0,
	DESC_RESOLUTION				 = 1,
	DESC_DEVICE_MARK			 = 2, 
	DESC_DEVICE_SERIESNUM	     = 3,
	DESC_DEVICE_BLANKSIZE        = 4,
	DESC_DEVICE_CHIPID           = 5,
	DESC_DEVICE_HARDWARE_VERSION = 6
} HV_DEVICE_INFO;



typedef enum tagHV_PERIDEV_CONTROL{ 
	PERIDEV_BICOLOR_LAMP1 = 0,
	PERIDEV_BICOLOR_LAMP2 =	1,
	PERIDEV_IR_EMITTER	  =	2,
	PERIDEV_LED			  =	3,
	PERIDEV_ARRIVE_CLEAR  =	4,
	PERIDEV_LED1		  =	5,
	PERIDEV_LED2		  =	6
} HV_PERIDEV_CONTROL;



typedef enum tagHV_COMMAND_CODE { 
	CMD_RESERVED0			= 0x00,
	CMD_RESERVED1			= 0x01,
	CMD_SPEC_FUN_INTERFACE1	= 0x80,
	CMD_HVAPI_CONTROL		= 0x100
} HV_COMMAND_CODE;


typedef enum tagHV_INTERFACE1_ID{ 
	COLOR_MODE = 4,
} HV_INTERFACE1_ID;



typedef enum tagHVSTATUS { 
	STATUS_OK							= 0, 
	STATUS_NO_DEVICE_FOUND				= -1,
	STATUS_DEVICE_HANDLE_INVALID		= -2,
	STATUS_HW_DEVICE_TYPE_ERROR			= -3,
	STATUS_HW_INIT_ERROR				= -4,
	STATUS_HW_RESET_ERROR				= -5,
	STATUS_NOT_ENOUGH_SYSTEM_MEMORY		= -6,
	STATUS_HW_IO_ERROR					= -7,
	STATUS_HW_IO_TIMEOUT				= -8,
	STATUS_HW_ACCESS_ERROR				= -9,
	////////////////////////////////////////////
	STATUS_OPEN_DRIVER_FAILED			= -10,
	STATUS_NOT_SUPPORT_INTERFACE		= -11,
	STATUS_PARAMETER_INVALID			= -12,
	STATUS_PARAMETER_OUT_OF_BOUND		= -13,
	STATUS_IN_WORK						= -14,
	STATUS_NOT_OPEN_SNAP				= -15,
	STATUS_NOT_START_SNAP				= -16,
	STATUS_FILE_CREATE_ERROR			= -17,
	STATUS_FILE_INVALID					= -18,
	STATUS_NOT_START_SNAP_INT			= -19,
	STATUS_INTERNAL_ERROR				= -20
} HVSTATUS;



#define HV_SUCCESS(status) ( (status) == STATUS_OK )

#ifdef _DEBUG
#define HV_VERIFY(status) \
		if((status) != STATUS_OK){ \
			::MessageBox(NULL, HVGetErrorString(status), \
						 "Error", MB_ICONWARNING | MB_OK);   \
		}     
#else
#define HV_VERIFY(status) (status)
#endif


#define HV_MESSAGE(status) \
		if((status) != STATUS_OK){ \
		::MessageBox(NULL, HVGetErrorString(status), \
		"Error message", MB_ICONWARNING | MB_OK);   \
		}    



typedef struct tagHV_SANP_INFO {
	HHV	hhv;
	int nDevice;
	int nIndex;
	void *pParam;
} HV_SNAP_INFO;


typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);


typedef struct tagHV_CHECK_PARAM {	
	BYTE byParam1;
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;
} HV_CHECK_PARAM;


typedef struct tagHV_INTERFACE1_CONTEXT{
	HV_INTERFACE1_ID	ID;
	DWORD				dwVal;
} HV_INTERFACE1_CONTEXT, *PHV_INTERFACE1_CONTEXT;


//HV define
//////////////////////////////////////////////////////////////////////////
/// Function ordinal numbers, used to construct driver control codes 
typedef enum tagHV_CONTROL_CODE {
	ORD_QUERY_LAST_STATUS_PRESENCE = 0x0000000C,
	ORD_GET_LAST_STATUS            = 0x0000000E

} HV_CONTROL_CODE;

//////////////////////////////////////////////////////////////////////////
/// The struct for the CMD_HVAPI_CONTROL function
typedef struct tagHVAPI_CONTROL_PARAMETER {
	HV_CONTROL_CODE code;
	void            *pInBuf;
	DWORD           dwInBufSize;
	void            *pOutBuf;	       
	DWORD           dwOutBufSize;   
	DWORD           *pBytesRet;   ///< cn:返回字节数，可以为NULL  en: return the number of byte, it can be NULL
} HVAPI_CONTROL_PARAMETER;


/// Result of the ORD_QUERY_LAST_STATUS_PRESENCE function
/*
   Query device last status function flag, indicating whether to enable or disable the function. 
   0:disable: 1:enable.
*/
typedef struct tagHV_RES_QUERY_LAST_STATUS_PRESENCE {
	DWORD : 29;  ///< Reserved
	DWORD Transfer : 1;	///< Last transfer status function flag. 0:disable: 1:enable.
	DWORD Snap     : 1;	///< Last snap status function flag. 0:disable: 1:enable.
	DWORD Control  : 1;	///< Last control status function flag. 0:disable: 1:enable.
} HV_RES_QUERY_LAST_STATUS_PRESENCE;

typedef enum tagHV_LAST_STATUS {
	HV_LAST_STATUS_ERROR_CONTROL = 0, // extended error information about HVSTATUS
	HV_LAST_STATUS_SNAP			 = 1, // snap status
	HV_LAST_STATUS_TRANSFER		 = 2, // transfer status about callback function, see HV_SNAP_PROC
} HV_LAST_STATUS;

typedef struct tagHV_ARG_GET_LAST_STATUS {
	HV_LAST_STATUS type;
} HV_ARG_GET_LAST_STATUS;

/// Return of the ORD_GET_LAST_STATUS
typedef struct tagHV_RES_GET_LAST_STATUS {
	DWORD status;          // Last status
} HV_RES_GET_LAST_STATUS;


/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

