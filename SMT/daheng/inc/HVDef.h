//----------------------------------------------------------------------------------
/**
\file		HVDef.h
\brief		This header file defines constant and external struct type
\version	V1.0.0.10
\date		2011/08/15

  Copyright (c) 2011 Daheng Imavision 
  All right reserved
*/
//----------------------------------------------------------------------------------


#if !defined(HV_HVDEF)         //
#define HV_HVDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
typedef HANDLE HHV;

/// the device type
typedef enum tagHVTYPE {
	UNKNOWN_TYPE = -1, ///< Invalid devcie type. 	
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
	HV1360UMTYPE = 42,

	HV1351UCTYPE = 56,
	HV2051UCTYPE = 57,
	HV3151UCTYPE = 58,
	HV1351UMTYPE = 59,
	GV400UCTYPE  = 60,
	GV400UMTYPE  = 61,
	HV5051UCTYPE = 62,
	HV5051UMTYPE = 63
} HVTYPE;

/// the color code for the device image
typedef enum tagHV_COLOR_CODE {
	COLOR_MONO8              = 0,  ///< Y only, Y=8bits
	COLOR_YUV8_4_1_1         = 1,  ///< Y=U=V=8bits, non compressed
	COLOR_YUV8_4_2_2         = 2,  ///< Y=U=V=8bits, non compressed
	COLOR_YUV8_4_4_4         = 3,  ///< Y=U=V=8bits, non compressed
	COLOR_RGB8               = 4,  ///< RGB 8 format, each component has 8bit data
	COLOR_MONO16             = 5,  ///< Y only, Y=16bits, non compressed(unsigned integer)
	COLOR_RGB16              = 6,  ///< RGB 16 format, each component has 16bit data
	COLOR_SIGNED_MONO16      = 7,  ///< Y only, Y=16bits, non compressed(signed integer)
	COLOR_SIGNED_RGB16       = 8,  ///< R=G=B=16bits, non compressed(signed integer)
	COLOR_RAW8               = 9,  ///< RAW 8 format, each pixel has 8bit data
	COLOR_RAW16              = 10, ///< RAW 16 format, each pixel has 16bit data
	COLOR_MONO10             = 128,///< Y only, each pixel has 16bit data, the low 10bit is valid
	COLOR_MONO10_NORMALIZED  = 129,///< Reserved
	COLOR_MONO10_PACKED      = 130,///< Y only, packed format, each pixel has 10bit data 
	COLOR_RAW10              = 131,///< RAW 10 format, each pixel has 16bit data, the low 10bit is valid
	COLOR_RAW10_NORMALIZED   = 132,///< Reserved
	COLOR_RAW10_PACKED       = 133,///< RAW 10 packed format, each pixel has 10bit data
	COLOR_MONO12             = 134,///< Y only, each pixel has 16bit data, the low 12bit is valid
	COLOR_MONO12_NORMALIZED  = 135,///< Reserved
	COLOR_MONO12_PACKED      = 136,///< Y only, packed format, each pixel has 12bit data 
	COLOR_RAW12              = 137,///< RAW 12 format, each pixel has 16bit data, the low 12bit is valid
	COLOR_RAW12_NORMALIZED   = 138,///< Reserved
	COLOR_RAW12_PACKED       = 139 ///< RAW 12 packed format, each pixel has 12bit data
		
} HV_COLOR_CODE;

/// Color Filter ID
typedef enum tagHV_COLOR_FILTER {
	COLOR_FILTER_RG_GB  = 0, ///< RGB primary color filter (RG/GB)
	COLOR_FILTER_GB_RG  = 1, ///< RGB primary color filter (GB/RG)
	COLOR_FILTER_GR_BG  = 2, ///< RGB primary color filter (GR/BG)
	COLOR_FILTER_BG_GR  = 3  ///< RGB primary color filter (BG/GR)
} HV_COLOR_FILTER;


/// the video mode
typedef enum tagHV_RESOLUTION { 
	RES_MODE0		= 0,  ///< Resolution 0
	RES_MODE1		= 1,  ///< Resolution 1
	RES_MODE2		= 2,  ///< Resolution 2
	RES_MODE3		= 3,  ///< Resolution 3
	RES_MODE4		= 4,  ///< Resolution 4
	RES_MODE5		= 5,  ///< Resolution 5
	RES_MODE6		= 6   ///< Resolution 6
} HV_RESOLUTION;


/// the image acquisition mode. 
typedef enum tagHV_SNAP_MODE { 
	CONTINUATION            = 0, ///< Acquire images continuously
	TRIGGER                 = 1, ///< Acquiring image controlled by standard trigger
	TRIGGER_EDGE            = 2, ///< Acquiring image controlled by edge event triggering
	TRIGGER_LEVEL           = 3, ///< Acquiring image controlled by level triggering
	TRIGGER_LEVEL_STANDARD  = 4  ///< Acquiring image controlled by standard level triggering
} HV_SNAP_MODE;

/// the trigger signal polarity
typedef enum tagHV_POLARITY { 
	LOW  = 0,
	HIGH = 1
} HV_POLARITY;

/// the image acquisition speed
typedef enum tagHV_SNAP_SPEED { 
	NORMAL_SPEED    = 0,    ///< Acquiring images at normal speed
	HIGH_SPEED      = 1     ///< Acquiring images at high speed
} HV_SNAP_SPEED;


///the color channel for controlling gains
typedef enum tagHV_CHANNEL { 
	RED_CHANNEL         = 0x10,	///< Red channel
	GREEN_CHANNEL1      = 0x11,	///< Green channe1
	GREEN_CHANNEL2      = 0x12, ///< Green channe2
	BLUE_CHANNEL        = 0x13  ///< Blue channel 
} HV_CHANNEL;

/// the controlling method of A/D conversion
typedef enum tagHV_ADC_CONTROL{ 
	ADC_BITS                       = 0,   ///< Controlling bits that are used for A/D conversion
	ADC_BLKLEVEL_CAL               = 1,   ///< Enable/disable to control the black level parameter
	ADC_BLKLEVEL_CAL_REDCHANNEL    = 0x10,///< Controlling the red channel of the black level
	ADC_BLKLEVEL_CAL_GREENCHANNEL1 = 0x11,///< Controlling the green channel 1 of the black level
	ADC_BLKLEVEL_CAL_GREENCHANNEL2 = 0x12,///< Controlling the green channel 2 of the black level
	ADC_BLKLEVEL_CAL_BLUECHANNEL   = 0x13 ///< Controlling the blue channel of the black level
} HV_ADC_CONTROL;

/// the level of A/D conversion
typedef enum tagHV_ADC_LEVEL{ 
	ADC_LEVEL0		= 0,    ///< Level 0
	ADC_LEVEL1		= 1,    ///< Level 1
	ADC_LEVEL2		= 2,    ///< Level 2
	ADC_LEVEL3		= 3,    ///< Level 3
	ADC_LEVEL4		= 4     ///< Level 4
} HV_ADC_LEVEL;

/// the controlling method of exposure
typedef enum tagHV_AEC_CONTROL{ 
	AEC_EXPOSURE_TIME   = 1, ///< Controlling exposure by exposure coefficient	
	AEC_SHUTTER_SPEED   = 2, ///< Controlling the shutter speed
	AEC_SHUTTER_UNIT    = 3	 ///< Controlling the unit of the shutter speed
} HV_AEC_CONTROL;

/// the unit of the shutter speed
typedef enum tagSHUTTER_UNIT{ 
	SHUTTER_US      = 0,    ///< The unit of the shutter speed is microsecond
	SHUTTER_MS      = 1     ///< The unit of the shutter speed is millisecond
}SHUTTER_UNIT_VALUE;

/// the mirror direction during the image acquisition
typedef enum tagHV_MIRROR_DIRECTION { 
	VERT_DIR	= 1,    ///< performing vertical mirror during image acquisition
	HOR_DIR		= 2,    ///< performing horizontal mirror during image acquisition
	FULL_DIR	= 3     ///< performing vertical and horizontal mirror during image acquisition
} HV_MIRROR_DIRECTION;

/// the device information of a camera
typedef enum tagHV_DEVICE_INFO { 
	DESC_DEVICE_TYPE             = 0,  ///< the device type of a camera
	DESC_RESOLUTION	             = 1,  ///< the resolution of a camera
	DESC_DEVICE_MARK             = 2,  ///< the device mark of a camera
	DESC_DEVICE_SERIESNUM	     = 3,  ///< the serial number of a camera
	DESC_DEVICE_BLANKSIZE        = 4,  ///< the available range of blanking interval
	DESC_DEVICE_CHIPID           = 5,  ///< the device chip id
	DESC_DEVICE_HARDWARE_VERSION = 6,  ///< the firmware version of camera
	DESC_DEVICE_BAYER_LAYOUT     = 11  ///< the bayer layout of image
} HV_DEVICE_INFO;


/// Define HV periphery device control
typedef enum tagHV_PERIDEV_CONTROL{ 
	PERIDEV_BICOLOR_LAMP1 = 0,		///< control the lamp1
	PERIDEV_BICOLOR_LAMP2 =	1,		///< control the lamp2
	PERIDEV_IR_EMITTER    =	2,		///< control the IR Emitter
	PERIDEV_LED           =	3,		///< control the LED
	PERIDEV_ARRIVE_CLEAR  =	4,		///< control the clear of arrive
	PERIDEV_LED1	      =	5,		///< control the LED1
	PERIDEV_LED2	      =	6		///< control the LED2
} HV_PERIDEV_CONTROL;


/// the controlling function of the camera
typedef enum tagHV_COMMAND_CODE { 
	CMD_RESERVED0                         = 0x00, ///< Reserved
	CMD_RESERVED1                         = 0x01, ///< Reserved
	CMD_RESERVED2                         = 0x02, ///< Reserved
	CMD_RESERVED3                         = 0x03, ///< Reserved

	CMD_SET_STROBE_SIGNAL_MODE            = 0x25, ///< Set the strobe signal mode(HV_SIGNAL_MODE)
	CMD_SET_EXPOSURE_MODE                 = 0x26, ///< Set the exposure mode(HV_EXPOSURE_MODE)
	CMD_SET_ROW_NOISE_CORR_CONTROL	      = 0x27, ///< Set the row noise correction control(HV_NOISECORR_CONTROL)
	CMD_RESET_SENSOR                      = 0x28, ///< Reset Sensor(HV_SENSOR_STATUS)
	CMD_RESERVED29                        = 0x29, ///< Reserved
	CMD_RESERVED30                        = 0x30, ///< Reserved
	
	CMD_SET_COLOR_CODE                    = 0x31, ///< Set the image color code (HV_COLOR_CODE)
	CMD_GET_COLOR_CODE                    = 0x32, ///< Get the image color code (HV_COLOR_CODE)

	CMD_GET_IMAGEINFO_ID_FRAME_COUNT      = 0x33, ///< Get the frame count from image info (DWORD)
	CMD_GET_IMAGEINFO_ID_FRAME_INTERVAL   = 0x34, ///< Get the frame interval from image info (DWORD)
	CMD_GET_IMAGEINFO_ID_SENSOR_TYPE      = 0x35, ///< Get the sensor type from image info (WORD)

	CMD_ENABLE_AUTO_GAIN_CTRL             = 0x36, ///< Enable or disable AGC(BOOL)
	CMD_GET_AGC_STATUS                    = 0x37, ///< Get the status of AGC:enable or disable? (BOOL)

	CMD_ENABLE_AUTO_SHUTTER_CTRL          = 0x38, ///< Enable or disable AEC(BOOL)
	CMD_GET_AEC_STATUS                    = 0x39, ///< Get the status of AEC:enable or disable? (BOOL)

	CMD_SET_AA_GRAY_VALUE                 = 0x40, ///< Set the expect gray value for AEC&AGC (DWORD)
	CMD_GET_AA_GRAY_VALUE                 = 0x41, ///< Get the expect gray value for AEC&AGC (DWORD)
	
	CMD_SET_AA_ROI                        = 0x42, ///< Set the region of interest for AEC&AGC (HV_RECT)
	CMD_GET_AA_ROI                        = 0x43, ///< Get the region of interest for AEC&AGC (HV_RECT)

	CMD_SET_AA_LIGHT_ENVIRONMENTS         = 0x44, ///< Set the light environments for AEC&AGC (HV_AA_LIGHT_ENVIRONMENTS)
	CMD_GET_AA_LIGHT_ENVIRONMENTS         = 0x45, ///< Get the light environments for AEC&AGC (HV_AA_LIGHT_ENVIRONMENTS)

	CMD_SET_AGC_ADJUST_RANGE              = 0x46, ///< Set the adjust range of AGC (HV_RANGE)
	CMD_GET_AGC_ADJUST_RANGE              = 0x47, ///< Get the maximum range to adjust (HV_RANGE)

	CMD_SET_AEC_ADJUST_RANGE              = 0x48, ///< Set the adjust range of AEC (HV_RANGE)
	CMD_GET_AEC_ADJUST_RANGE              = 0x49, ///< Get the maximum range to adjust (HV_RANGE)

	CMD_GET_AGC_CURRENT_VALUE             = 0x50, ///< Get the current value of AGC (DWORD)
	CMD_GET_AEC_CURRENT_VALUE             = 0x51, ///< Get the current value of AEC (DWORD)

	CMD_ENABLE_AUTO_WHITEBALANCE          = 0x52, ///< Enable or disable AWB (BOOL)
	CMD_GET_AUTO_WHITEBALANCE_STATUS      = 0x53, ///< Get the status of AWB:enable or disable? (BOOL)

	CMD_ENABLE_COLOR_CORRECTION           = 0x54, ///< Enable or disable color correction (BOOL)
	CMD_GET_COLOR_CORRECTION_STATUS       = 0x55, ///< Get the status of color correction:enable or disable? (BOOL)

	CMD_GET_SNAPSPEED_COEFF_RANGE		  = 0x56, ///< Get the adjust range of the snap speed coefficient (HV_RANGE)
	CMD_SET_SNAPSPEED_COEFFICIENT  		  = 0x57, ///< Set the coefficient of the snap speed(DWORD)
	CMD_GET_SNAPSPEED_COEFFICIENT  		  = 0x58, ///< Get the coefficient of the snap speed(DWORD)

	CMD_SPEC_FUN_INTERFACE1               = 0x80, ///< special function(HV_INTERFACE1_ID)

	CMD_HVAPI_CONTROL                     = 0x100 ///< function defined by HV_CONTROL_CODE
} HV_COMMAND_CODE;

/// Argument for the adjust rect
typedef struct tagHV_RECT{
    long left;      ///< the left of an image
    long top;       ///< the top of an image
    long right;     ///< the right of an image
    long bottom;    ///< the bottom of an image
}HV_RECT;

/// Argument for the CMD_SET_AA_LIGHT_ENVIRONMENTS
typedef enum tagHV_AA_LIGHT_ENVIRONMENTS{
    NATURE_LIGHT = 0,	        ///< direct current or natural light 
    AC_50HZ = 1,                ///< 50Hz alternating current
    AC_60HZ = 2                 ///< 60Hz alternating current
} HV_AA_LIGHT_ENVIRONMENTS;

/// Argument for adjust range
typedef struct tagHV_RANGE  
{
	DWORD nMinValue;	///< the min value
	DWORD nMaxValue;	///< the max value
}HV_RANGE;

///Argument for the CMD_SPEC_FUN_INTERFACE1
typedef enum tagHV_INTERFACE1_ID{ 
	COLOR_MODE = 4,
} HV_INTERFACE1_ID;

/// the mode of strobe signals.
typedef enum tagHV_SIGNAL_TYPE{
	SIGNAL_IMPULSE = 0,
	SIGNAL_LEVEL   = 1
} HV_SIGNAL_TYPE;

/// argument for the CMD_SET_STROBE_SIGNAL_MODE
typedef struct tagHV_SIGNAL_MODE{
	int Polarity;      ///< 0:LOW ;1: HIGH
	int Mode;    //0:SIGNAL_IMPULSE; 1: SIGNAL_LEVEL
}HV_SIGNAL_MODE;

///the camera's exposure modes, argument for the CMD_SET_EXPOSURE_MODE
typedef enum tagHV_EXPOSURE_MODE{
	SEQUENCE     = 0,  ///< the sequence exposure mode 
    SIMULTANEITY = 1   ///< the simultaneity exposure mode
} HV_EXPOSURE_MODE;

/// the status of sensor, argument for the CMD_RESET_SENSOR
typedef enum tagHV_SENSOR_STATUS{
	RESET = 0         ///< reset sensor
}HV_SENSOR_STATUS;

typedef enum tagHV_NOISECORR_CONTROL{
	NOISECORR_ENABLE = 0,   
    NOISECORR_DISABLE = 1
} HV_NOISECORR_CONTROL;

/// the status code
typedef enum tagHVSTATUS { 
	STATUS_OK                           = 0, 
	STATUS_NO_DEVICE_FOUND				= -1,
	STATUS_DEVICE_HANDLE_INVALID        = -2,
	STATUS_HW_DEVICE_TYPE_ERROR			= -3,
	STATUS_HW_INIT_ERROR				= -4,
	STATUS_HW_RESET_ERROR				= -5,
	STATUS_NOT_ENOUGH_SYSTEM_MEMORY     = -6,
	STATUS_HW_IO_ERROR                  = -7,
	STATUS_HW_IO_TIMEOUT				= -8,
	STATUS_HW_ACCESS_ERROR				= -9,
	////////////////////////////////////////////
	STATUS_OPEN_DRIVER_FAILED			= -10,
	STATUS_NOT_SUPPORT_INTERFACE        = -11,
	STATUS_PARAMETER_INVALID			= -12,
	STATUS_PARAMETER_OUT_OF_BOUND       = -13,
	STATUS_IN_WORK	                    = -14,
	STATUS_NOT_OPEN_SNAP				= -15,
	STATUS_NOT_START_SNAP				= -16,
	STATUS_FILE_CREATE_ERROR			= -17,
	STATUS_FILE_INVALID	                = -18,
	STATUS_NOT_START_SNAP_INT			= -19,
	STATUS_INTERNAL_ERROR				= -20,
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


/**
 the structure defines a set of data to store the working status 
 of a camera when grabbing images into system memory
*/
typedef struct tagHV_SANP_INFO {
	HHV	hhv;		///<Handle to current camera
	int nDevice;	///<Index of working camera; the index starts with 1
	int nIndex;		///<Index of buffer used by current camera; the index starts with 0
	void *pParam;	///<Pointer to a parameter defined by user
} HV_SNAP_INFO;

/// define the callback function
typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);

/// Argument for HVCheckDevice
typedef struct tagHV_CHECK_PARAM {	
	BYTE byParam1;
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;
} HV_CHECK_PARAM;

/// Argument for the CMD_SPEC_FUN_INTERFACE1
typedef struct tagHV_INTERFACE1_CONTEXT{
	HV_INTERFACE1_ID	ID;
	DWORD               dwVal;
} HV_INTERFACE1_CONTEXT, *PHV_INTERFACE1_CONTEXT;


/// Function ordinal numbers, used to construct driver control codes 
typedef enum tagHV_CONTROL_CODE{
	ORD_QUERY_LAST_STATUS_PRESENCE    = 0x0000000C,
	ORD_GET_LAST_STATUS               = 0x0000000E
}HV_CONTROL_CODE;


/// The struct for the CMD_HVAPI_CONTROL function
typedef struct tagHVAPI_CONTROL_PARAMETER {
	HV_CONTROL_CODE code;
	void            *pInBuf;      ///< the input buffer
	DWORD           dwInBufSize;  ///< the size of input buffer
	void            *pOutBuf;     ///< the output buffer      
	DWORD           dwOutBufSize; ///< the size of output buffer
	DWORD           *pBytesRet;   ///< return the actual size of pOutBuf(bytes)
}HVAPI_CONTROL_PARAMETER;


/**
   Result of the ORD_QUERY_LAST_STATUS_PRESENCE function.
   Query device last status function flag, indicating whether to enable or disable the function. 
   0:disable: 1:enable.
*/
typedef struct tagHV_RES_QUERY_LAST_STATUS_PRESENCE{
	DWORD : 29;         ///< Reserved
	DWORD Transfer : 1;	///< Last transfer status function flag. 0:disable: 1:enable.
	DWORD Snap     : 1;	///< Last snap status function flag. 0:disable: 1:enable.
	DWORD Control  : 1;	///< Last control status function flag. 0:disable: 1:enable.
}HV_RES_QUERY_LAST_STATUS_PRESENCE;

/// the types of camera's working status
typedef enum tagHV_LAST_STATUS {
	HV_LAST_STATUS_ERROR_CONTROL = 0, ///< extended error information about HVSTATUS
	HV_LAST_STATUS_SNAP          = 1, ///< snap status
	HV_LAST_STATUS_TRANSFER	     = 2, ///< transfer status about callback function, see HV_SNAP_PROC	
}HV_LAST_STATUS;

/**
 the input parameters of "ORD_GET_LAST_STATUS",
 describing the type of camera's working status
*/
typedef struct tagHV_ARG_GET_LAST_STATUS{
	HV_LAST_STATUS type;		///< The type of camera's working status, reference to HV_LAST_STATUS
}HV_ARG_GET_LAST_STATUS;

/// the return code of last status
typedef enum tagHV_LAST_STATUS_RETURNCODE {
	STATUS_BAD_FRAME		 = 0xE0040005,	///< the code for bad frames
}HV_LAST_STATUS_RETURNCODE;

/// Return of the ORD_GET_LAST_STATUS
typedef struct tagHV_RES_GET_LAST_STATUS{
	HV_LAST_STATUS_RETURNCODE status;     ///< Last status
}HV_RES_GET_LAST_STATUS;


/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

