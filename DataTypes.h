#ifndef DATATYPES_H
#define DATATYPES_H
#include<QThread>
#include<QWidget>
#include<stdio.h>
#include<stdlib.h>
#include<QFrame>
#include<QPalette>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include<QLabel>
#include<stdint.h>

#define ACCELEROMETER_SENSOR 32
#define HYDROPHONE_SENSOR 29
#define MAX_SCU_NO 10
#define MAX_PACKET_NO 32
#define MAX_SENSOR_GROUP 4
#define SCU_PORT_NO 5919
#define SONAR_HEALTH_STATUS_PORT 50027
#define NO_OF_SENSOR 61
#define NO_OF_CHANNEL 16
#define RawDataBufferSize 5
#define RawDataBufferSampleSize 16384
#define NO_OF_TRACK 64
#define REC_DATA_SEGLEN 4096

typedef __int64_t nplint64;
typedef __uint64_t npluint64;
typedef __int32_t nplint32;
typedef __uint32_t npluint32;
typedef __int16_t nplint16;
typedef __uint16_t npluint16;
typedef __int8_t nplint8;
typedef __uint8_t npluint8;
typedef char nplchar8;
typedef unsigned char npluchar8;
typedef float nplfloat32;
typedef double npldouble64;

#define PC_IP_ADDR "192.168.1.70"
//#define PC_IP_ADDR "10.7.21.61"       			    // PC IP setting
#define MULTICAST_IP_GROUP_ADDR "239.192.2.31"          // Multicast IP Group
#define PORT_NO  5235
#define TRANS_PORT_NO 55555

#define NO_OF_CH 16
#define SAMPLES_PER_CH 128

#define FDS          0x1B //Destination ID for FDS
#define UDP 		0x01

#define ONA_1     		0x3D
#define FDS_IP_ADDRESS "10.7.21.27"
//#define FDS_IP_ADDRESS "127.0.0.1"

#define FDS       0x1B
#define FDS_PORT 50027

#define NPL_POWERPC_PROCESSOR 0x01
#define NPL_PENTIUM_PROCESSOR 0x02

//#define NPL_PROCESSOR  NPL_POWERPC_PROCESSOR
#define NPL_PROCESSOR  NPL_PENTIUM_PROCESSOR

#define NO_OF_SUBSYSTEMS 	256
#define UCS_DATA_LEN 255

#define FDS_PORT 50027
#define SPMS_1     		0xF1
#define FDS_ONA_1                  0x121// HMIs
#define NUM_HEADER_SPARES          13
#define HEALTH_TIMEOUT_SEC 5
const npluint32 PERIODIC_HEALTH_DATA = 				0xF1;   ///< Periodic health data from individual subsystems

const unsigned long INVALID_TEMP =  0xFFFF;
const unsigned long INVALID_MEM =   0x0;
const npluint32 HMI_TO_FDS_REQUEST = 0xF3;

#define NO_OF_AMPLIFIER_ROOM_CABINETS 6
#define NO_OF_SONAR_ROOM_CABINETS 3
#define NO_OF_CAPSULE_HSU 12
#define NO_OF_TXC_CARDS 10
#define NO_OF_ETH_JBS   2
#define NO_TOTAL_CAB_BLOCKS (NO_OF_AMPLIFIER_ROOM_CABINETS + NO_OF_SONAR_ROOM_CABINETS + NO_OF_CAPSULE_HSU + NO_OF_ETH_JBS)
#define MAX_NO_PSS_HSU 6


#define MAX_NO_PSS_HSU 6
#define NO_OF_AMPLIFIER_ROOM_CABINETS 6
#define NO_OF_SONAR_ROOM_CABINETS 3
#define NO_OF_CAPSULE_HSU 12
#define NO_OF_TXC_CARDS 10
#define NO_OF_ETH_JBS   2
#define NO_TOTAL_CAB_BLOCKS (NO_OF_AMPLIFIER_ROOM_CABINETS + NO_OF_SONAR_ROOM_CABINETS + NO_OF_CAPSULE_HSU + NO_OF_ETH_JBS)

//ulCategory = HMI_TO_FDS_REQUEST;
//ulSubItem
#define BRIEF_HEALTH_REGISTER_REQUEST       0x01
#define FDS_ALL_UNREGISTER_REQUEST          0x0A

typedef struct
{
    int NPOL_HEADER[32];
    int SEQ_NO;
    char ACTIVE_CHANNEL_NO;
    char Version;
    short int SCU_ID;
    short int TEMP_SENSOR2;
    short int TEMP_SENSOR1;
    short int FPGA_TEMP_SENSOR;
    short int TEMP_SENSOR3;
    short int TEST_MODE;
    short int CHANNEL_STATUS;
    int FPGA_STATUS_FLAG;
    int POWER_SUPPLY_STATUS;
    int32_t LINK_STATUS;
    int32_t SPARE_FEILD[3];
    int32_t CHANNEL_GAIN_INFO[16];
    int32_t REGISTER[32];
    int32_t SPARE[5];
    int32_t DataBuffer[2048];

} DATA_PACKET_STRUCT;


typedef struct
{
     bool SCU_LINK_STATUS[MAX_SCU_NO];
     int16_t CH_SCU_NO[NO_OF_SENSOR];
     int32_t CH_GAIN_Rx[NO_OF_SENSOR];
     int16_t CH_POS_NO[NO_OF_SENSOR];
     int16_t CH_IN_SCU[MAX_SCU_NO][NO_OF_CHANNEL];
     int32_t CH_GAIN_SCU[MAX_SCU_NO][NO_OF_CHANNEL];
} Link;

typedef struct
{
   bool PostStatus[RawDataBufferSize];
   bool PostRecStatus[RawDataBufferSize];
   int32_t FrontPointer;
   int32_t RearPointer;
   int32_t FrontRecPointer;
   int32_t RearRecPointer;
   int32_t SampleNumber;
   int32_t SampleCountNumber;
   int32_t RawDataIntBuffer[RawDataBufferSize][RawDataBufferSampleSize];
   float RawDataBuffer[RawDataBufferSize][RawDataBufferSampleSize];
} ControlData;


typedef struct
{
   bool DelSpec_PostStatus[RawDataBufferSize];
   bool DelSpec_PostRecStatus[RawDataBufferSize];
   int32_t DelSpec_FrontPointer;
   int32_t DelSpec_RearPointer;
   int32_t DelSpec_FrontRecPointer;
   int32_t DelSpec_RearRecPointer;
   int32_t DelSpec_SampleNumber;
   int32_t DelSpec_SampleCountNumber;
   int32_t DelSpec_RawDataIntBuffer[RawDataBufferSize][RawDataBufferSampleSize];
   float DelSpec_RawDataBuffer[RawDataBufferSize][RawDataBufferSampleSize];
} DelayedSpectrum_Data;

typedef struct
{
    float TFrequency;
    float TAmplitude;
    bool Init_CH_No[NO_OF_SENSOR][8];
    float CH_NO_Freq[NO_OF_SENSOR][8];
    float CH_NO_Amp[NO_OF_SENSOR][8];
    bool ThresholdWarningFlag[NO_OF_SENSOR][8];
    bool ThresholdWarningFlagEnable[NO_OF_SENSOR][8];
    int32_t xThresholdPos;
    int32_t yThresholdPos;
   //int16_t  ThresholdCHNo;

} ChannelThreshold;

typedef struct
{
    float Frequency;
    float Amplitude;
    int32_t xTrackPos;
    int32_t yTrackPos;
} ChannelTrack;


typedef struct
{
    float Frequency;
    float Amplitude;
    bool TrackStatus;
    bool TrackMiss;
    bool TrackEnable;
    int32_t xPos;
    int32_t yPos;
    int16_t CH_NO[NO_OF_SENSOR];
    int16_t INT_CH_NO;
} ChannelTrackValue;



typedef struct UDPTRXData1
{
    unsigned int NPOL_HEADER[32];
    unsigned int FreqTrackObjectID;
    unsigned int Object1_Length;
    unsigned int Object1_Status;
    unsigned int TRACK1_FREQ[128];
    unsigned int SPARE1[4];
    unsigned  int Object1_Trailer;
    unsigned int PassiveTrackObjectID;
    unsigned int Object2_Length;
    unsigned int Object2_Status;
    unsigned int PassiveEnergyLevel[5];
    unsigned int SPARE2[4];
    unsigned int Object2_Trailer;
    unsigned int CavitationTrackObjectID;
    unsigned int Object3_Length;
    unsigned int Object3_Status;
    unsigned int CavitationThresholdFlag;
    unsigned int SPARE3[4];
    unsigned int Object3_Trailer;
} UDPTRXData1;


typedef struct Npol_header
{
    unsigned long pkt_Length;
    unsigned long Src;
    unsigned long Dst;
    unsigned long Catgry;
    unsigned long Sub_Catgry;
    unsigned long Protocol;
    unsigned long No_of_Objs_in_pkt;
    unsigned long No_of_Pkt_Fragment;
    unsigned long Pkt_fragment_no;
    unsigned long Flag ;
    unsigned long Seq_no;
    unsigned long Pkt_header;
    unsigned long Endian_Type ;
    unsigned long Time_Stamp[4];   //Which is not supported mentioned in src_npol
    unsigned long Spare[14] ;
    unsigned long Chk_Sum;
} npol_header_t;


typedef struct Host_Control_Pkt
{
    npol_header_t npol_hdr;
    unsigned long obj_id;
    unsigned long obj_len;
    unsigned long obj_valid;
    unsigned char save_flag;
    unsigned char scu_id;
    unsigned short agc_mgc_flag;
    unsigned short thrs_flag_act_deact;
    unsigned short mode_of_op;
    unsigned long master_flag;
    unsigned long spr[3] ;
    unsigned long data[32];
    unsigned long reg[32];
    unsigned long obj_trail ;

} Host_Control_pkt_t;



/*****************************************************************
Details of Navigational Data Packet in ISS
Name :ISSNavDataPacketType
Rate : 1sec
UDP Port : 55555
Broadcast I.P : 10.7.21.255
******************************************************************/
/// Category
//const unsigned int EXTERNAL_DATA =	0xB0;

/// Sub-Category
//const unsigned int ISS_NAV_DATA_ESI = 0x40; /// Nav Data Packet from ESI-NAV , UDP Broadcast(IP:10.7.21.255,Port:55555) , Periodic (1 sec)
//const unsigned int ISS_NAV_DATA_HMI = 0x41; /// Nav Data Packet from HMI , Broadcast , Aperiodic , Note : System Mode and parameter modes shall be set from this packet 						 /// by all subsystems
//const unsigned int ISS_NAV_DATA_SNS = 0x42; /// Nav Data Packet from SNS , Unicast to ESI-NAV , Periodic (1 sec)

/// Object IDs
//const unsigned int ISS_NAV_DATA_ESI_OBJ = 0xB040;/// Nav Data Packet from ESI-NAV
//const unsigned int ISS_NAV_DATA_HMI_OBJ = 0xB041;/// Nav Data Packet from HMI
//const unsigned int ISS_NAV_DATA_SNS_OBJ = 0xB042;/// Nav Data Packet from SNS

#define NUM_HEADER_SPARES          13

/**
  \brief Flags included in packet header
 */
typedef union
{
    npluint32 ulVal;
#if __BYTE_ORDER == __BIG_ENDIAN
    struct
    {
    npluint32 bitDummy : 23;
    npluint32 bitProjId : 8;
    npluint32 bitInUseFlag : 1;
    }bit;
#else
    struct
    {
    npluint32 bitInUseFlag : 1;
    npluint32 bitProjId : 8;
    npluint32 bitDummy : 23;
    }bit;
#endif
} FlagType;
typedef union
{
    npluint32 ulVal;
#if __BYTE_ORDER == __BIG_ENDIAN
    struct
    {
        unsigned int bitSubDestId:16;
        unsigned int bitDestId:16;
    } bit;
#else
    struct
    {
        unsigned int bitDestId:16;
        unsigned int bitSubDestId:16;
    } bit;
#endif
} DestIdType;

//typedef union
//{
//    unsigned int ulVal;
//#if __BYTE_ORDER == __BIG_ENDIAN
//    struct
//    {
//        short uiYear;
//        char ucMonth;
//        char ucDay;
//    } bit;
//#else
//    struct
//    {
//        char ucDay;
//        char ucMonth;
//        short uiYear;
//    } bit;
//#endif
//} DateType;


//typedef union
//{
//    unsigned int ulVal;
//#if __BYTE_ORDER == __BIG_ENDIAN
//    struct
//    {
//       short uiHour;
//        char ucMinute;
//        char ucSecond;
//    } bit;
//#else
//    struct
//    {
//        char ucSecond;
//        char ucMinute;
//        char uiHour;
//    } bit;
//#endif
//} TimeType;

typedef struct
{
    npluint8 ucDay;
    npluint8 ucMonth;
    npluint16 uiYear;

}DateType;

typedef struct
{
    npluint8 ucSecond;
    npluint8 ucMinute;
    npluint16 uiHour;

}TimeType;
/**
  \brief Packet Header which is common for all Network packets
  */
typedef struct
{
    unsigned int ulPktLen;			///< packet length
    unsigned int ulSrc;			///< source id
    DestIdType stDest;
    unsigned int ulCategory;		///< category id of the packet
    unsigned int ulSubItem;		///< subitem id of the packet
    unsigned int ulProtocol;		///< protocol used for transmission
    unsigned int ulNoOfObj;		///< No. of objects
    unsigned int ulNoOfFragments;	///< No Of fragments (in case of data that won't fit in to a single packet)
    unsigned int ulFragmentNo;		///< Fragment Number
    FlagType stFlag;			///< Flag which indicates that the packet is in use ie waiting for processing to be done or transmitted
    unsigned int ulSequenceNo;		///< Sequence Number can be manplint32ained for each type of packet by the application
    unsigned int ulAAAA5555;		///< Flag . Value = 0xAAAA5555
    unsigned int ulEndianess;		///< 1=Little-Endian, 2=Big-Endian
    DateType stDate;			///< Byte-0=Day, Byte-1=Month, Bytes 2 and 3 = year
    TimeType stTime;			///< Byte-0=Seconds, Byte-1=Minutes, Bytes 2 and 3 = Hours
    unsigned int ulTimeMicroSec;	///< Microseconds
    unsigned int ulTimeSpare;		///< Spares
    unsigned int ulVersion;		///< Reserved
    unsigned int ulSpare [NUM_HEADER_SPARES];
    unsigned int ulChecksum;		///< Header checksum of all fields
} NetworkPacketHeaderType;


/**
Object Data Header

Header that is used before each Object in a data packet. This is a generic
structure used as part of all data packets.
 */
typedef struct
{
    unsigned int ulObjId;      //!< Object ID
    unsigned int ulObjLen;     //!< Object length
    unsigned int ulObjStatus;	//!< Object status (bit 0 - validity, bit 1 - test mode)
                                //!< See bit masks defined in ProjectDefs.h
} ObjDataHeader;

/**
Object Data Trailer

Trailer that is used before each Object in a data packet. This is a generic
structure used as part of all data packets.
 */
typedef struct
{
    unsigned int ulChecksum;	//!< Checksum of data area and object header
} ObjDataTrailer;

typedef struct
{
    unsigned int Year;/// [2010 to 2070]
    unsigned int Month;/// [1 to 12]
    unsigned int Day;/// [1 to 31]
    unsigned int Hour;/// [0 to 23]
    unsigned int Minute;/// [0 to 59]
    unsigned int Second;/// [0 to 59]
    unsigned int Millisec;/// [0 to 999]

} NavTimeType;

/*  NAV Modes per parameter:

    AUTO:
        If(System Mode == SYSMODE_ANALOG_TEST or SYSMODE_REPLAY or SYSMODE_BEAM_PATTERN_CHECK or SYSMODE_TRAINING_SIM)
            ESI doesnt send out NAV data
        If (System Mode  == NORMAL)
                    ESI takes data from NAV
                If (System Mode  == DIGITAL_SNS or ANALOG_SNS)
            then if(TSS-Steering-Mode == INTERNAL)
                            ESI takes data from SNS-TSS
                    else if (TSS-Steering-Mode == EXTERNAL)
                            ESI takes data from ESI-NAV

    MANUAL:		If(System Mode == SYSMODE_ANALOG_TEST or SYSMODE_REPLAY or SYSMODE_BEAM_PATTERN_CHECK or SYSMODE_TRAINING_SIM)
            ESI doesnt send out NAV data
            else
            ESI takes HMI manual data and sends out
*/
typedef struct
{
 /*   /// 1 = AUTO, 2 = MANUAL
    unsigned int ulDateTimeMode;
    unsigned int ulHeadingMode;
    unsigned int ulRollMode;
    unsigned int ulPitchMode;
    unsigned int ulLatitudeMode;
    unsigned int ulLongitudeMode;
    unsigned int ulCourseOverGroundMode;
    unsigned int ulSpeedOverGroundMode;
    unsigned int ulDivingDepthMode;
    unsigned int ulWaterDepthMode;
    unsigned int ulWaterSoundVelocityMode;
    unsigned int ulModeSpares[5];
    /// 1 = VALID, 0 = INVALID
    unsigned int ulDateTimeVld;
    unsigned int ulHeadingVld;
    unsigned int ulRollVld;
    unsigned int ulPitchVld;
    unsigned int ulLatitudeVld;
    unsigned int ulLongitudeVld;
    unsigned int ulCourseOverGroundVld;
    unsigned int ulSpeedOverGroundVld;
    unsigned int ulDivingDepthVld;
    unsigned int ulWaterDepthVld;
    unsigned int ulWaterSoundVelocityVld;
    unsigned int ulVldSpares[5];
    NavTimeType stNavTime;

    float fHeading;			/// degrees , [0-360[
    float fRoll;				/// degrees , [-90 to +90[
     float fPitch;				/// degrees ,[-90 to +90[
    float fLatitude;			/// Degrees,[-90 to +90[
   float  fLongitude;			/// Degrees,[-180 to +180[
     float fCourseOverGround;	/// degrees , [0 to 360[
    float  fSpeedOverGround;	/// knots ,[0 to 127.392[ , Note: [0 to 65.536[ in m/s, 1m/s=1.9438 knots
     float  fDivingDepth;		/// in metres ,[0 to 655.36[
    float fWaterDepth;			/// in metres ,[0 to 12000[
    float  fWaterSoundVelocity;	/// in m/s ,[1400 to 1024[
     float  fSpares[5];


     */

       npluint32 ulDateTimeMode;
       npluint32 ulHeadingMode;
       npluint32 ulRollMode;
       npluint32 ulPitchMode;
       npluint32 ulLatitudeMode;
       npluint32 ulLongitudeMode;
       npluint32 ulCourseOverGroundMode;
       npluint32 ulSpeedOverGroundMode;
       npluint32 ulDivingDepthMode;
       npluint32 ulWaterDepthMode;
       npluint32 ulWaterSoundVelocityMode;
       npluint32 ulLogSpeedMode;
       npluint32 ulModeSpares[4];
       /// 1 = VALID, 0 = INVALID
       npluint32 ulDateTimeVld;
       npluint32 ulHeadingVld;
       npluint32 ulRollVld;
       npluint32 ulPitchVld;
       npluint32 ulLatitudeVld;
       npluint32 ulLongitudeVld;
       npluint32 ulCourseOverGroundVld;
       npluint32 ulSpeedOverGroundVld;
       npluint32 ulDivingDepthVld;
       npluint32 ulWaterDepthVld;
       npluint32 ulWaterSoundVelocityVld;
       npluint32 ulLogSpeedVld;
       npluint32 ulVldSpares[4];

       NavTimeType stNavTime; 			/// Date and Time
       nplfloat32 fHeading;			/// degrees , [0-360[
       nplfloat32 fRoll;				/// degrees , [-90 to +90[
       nplfloat32 fPitch;				/// degrees ,[-90 to +90[
       nplfloat32 fLatitude;			/// Degrees,[-90 to +90[
       nplfloat32 fLongitude;			/// Degrees,[-180 to +180[
       nplfloat32 fCourseOverGround;	/// degrees , [0 to 360[
       nplfloat32 fSpeedOverGround;	/// knots ,[0 to 127.392[ , Note: [0 to 65.536[ in m/s, 1m/s=1.9438 knots
       nplfloat32 fDivingDepth;		/// in metres ,[0 to 655.36[
       nplfloat32 fWaterDepth;			/// in metres ,[0 to 12000[
       nplfloat32 fWaterSoundVelocity;	/// in m/s ,[1400 to 1024[
       nplfloat32 fLogSpeed;           /// in knots , [-127.392 to +127.392[ ,Note :[-65.536 to +65.536[ in m/s
       nplfloat32 fSpares[4];			/// TODO: Are spares sufficient?


}IssNavDataType;

typedef struct ///grapg1 plot picker
{
  int CaviationEnable;
  int NavFlag;
  int Year;
  int Month;/// [1 to 12]
  int Day;/// [1 to 31]
  int Hour;/// [0 to 23]
  int Minute;/// [0 to 59]
  int Second;/// [0 to 59]
  int Millisec;/// [0 to 999]
  char CourseValue[40];
  char SpeedValue[40];
  char Diving_Depth[40];
  char SoundVelocity[40];
  char Latitude[40];
  char Longitude[40];
  char Logspeed[40];
  char Heading[40];
  char Date[150];
  char Time[150];
  char WaterfallTime1[150];
  char WaterfallTime2[150];
  char WaterfallTime3[150];
  char WaterfallTime4[150];
  int Regime_Request[6];
  int CurrentChannelNo;
  int CurrentHour;
  int CurrentMin;
  int CurrentSec;
  int ChangeHour;
  int ChangeMin;
  int ChangeSec;

} SonarDataStructure;

typedef struct
{
    ObjDataHeader stObjDataHeader;
    IssNavDataType stIssNavData;
    ObjDataTrailer stObjDataTrailer;
} ISSNavDataObjectType;

/* Generic NAV Data Packet Structure to be used in ISS Sonar, transmitted from ESI ,HMI and SNS
 */

typedef struct
{
    NetworkPacketHeaderType stNetHeader;                    ///< Network header
    ISSNavDataObjectType  stISSNavDataObject;
}ISSNavDataPacketType;



typedef struct
{
    unsigned long ulCriticalHealth;
    unsigned long ulCtdHealth;
    unsigned long ulCicsHealth;
    unsigned long ulNavHealth;
    unsigned long ulSpare[4];
}ExtAndCriticalSysHealth;
/*
typedef enum
{
    FDS_UNHEALTHY     = 1,//0b000001,
    FDS_FULLY_HEALTHY = 2,//0b000010,
    FDS_DEGRADED      = 4,//0b000100,
    FDS_UNKNOWN       = 8,//0b001000,
    FDS_NOT_PRESENT   = 16,//0b010000
    FDS_NOT_APPLICABLE = 32//0b0100000
}FdsHealthStatusType;

*/

typedef enum
{
    FDS_UNHEALTHY     = 1,//0b000001,
    FDS_FULLY_HEALTHY = 2,//0b000010,
    FDS_DEGRADED      = 4,//0b000100,
    FDS_UNKNOWN       = 8,//0b001000,
    FDS_NOT_PRESENT   = 16,//0b010000
    FDS_NOT_APPLICABLE = 32//0b0100000
}FdsHealthStatusType;

//typedef struct
//{
//    unsigned long ulSonarHealth;
//    //FdsHealthStatusType eCabinetHealth[NO_TOTAL_CAB_BLOCKS];
// //   ExtAndCriticalSysHealth stExtAndCriticalSysHealth;
// //   unsigned long ulDDROpStatus;
// //   unsigned long ulHsuCabMasterSlaveStatus[NO_OF_CAPSULE_HSU];
//  //  unsigned long ulHsuCabSyncOutOfSyncStatus[NO_OF_CAPSULE_HSU];
//} FdsBriefHealthData;
//typedef struct FdsBriefHealthPacketType
//{
//    NetworkPacketHeaderType stNetHeader;
//    FdsBriefHealthData stFdsBriefHealthData;
//} FdsBriefHealthPacketType;



typedef enum
{
    NPOL_SUBSYSTEM = 1,
    NPOL_MULTICORE_HARDWARE,
    NPOL_SWITCH_SBC,
    NPOL_CABINET,
    NPOL_OTHER,
    NPOL_UNUSED_SLOT,
    NPOL_FE_CARD,
    NPOL_OAS_PA_CARD,
    NPOL_NON_HEALTH_CARD
}HardwareComponentType;



typedef struct
{
    unsigned long ulSonarHealth;
    FdsHealthStatusType eCabinetHealth[NO_TOTAL_CAB_BLOCKS];
    ExtAndCriticalSysHealth stExtAndCriticalSysHealth;
    unsigned long ulDDROpStatus;
    unsigned long ulHsuCabMasterSlaveStatus[NO_OF_CAPSULE_HSU];
    unsigned long ulHsuCabSyncOutOfSyncStatus[NO_OF_CAPSULE_HSU];
    int iPssHsuGain[MAX_NO_PSS_HSU];
    int iOasHsuGain;
    int iInterceptB2Gain;
    int iInterceptB3Gain;
    int iInterceptB1AftGain;
    int iInterceptB2AftGain;
} FdsBriefHealthData;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;
    FdsBriefHealthData stFdsBriefHealthData;
} FdsBriefHealthPacketType;

typedef struct
{

int SpectrumZoomIndex;
int SpectrumZoom2Index;
}Zoom_Indexing;


typedef struct
{
    unsigned long ulFreeMemInKb;
    unsigned long ulTotalMemInKb;
    unsigned long ulBoardUpTimeSeconds;
    int iTemperature[4];
    unsigned long ulSpare[9];
}InfoProcHardHealth;

typedef struct
{

    unsigned long ulAppUpTimeSeconds;
    DateType stAppBuildDate;
    unsigned long ulMajVer;
    unsigned long ulMinVer;
    unsigned long ulBuildNum;
    unsigned long ulSpare[3];
}InfoProcSoftHealth;

typedef struct
{
    unsigned int uiLinkStatus[10];          //SCU link status 1 indicates Link OK 0 indicate Link failiure
    unsigned int uiSensorHealth[62];   //Spares for future use
    unsigned int uiSpare[5];   //Spares for future use

}SpmsAppSepcHealth;

typedef struct
{
    InfoProcHardHealth stHardHealth;
    InfoProcSoftHealth stSoftHealth;
    SpmsAppSepcHealth stAppSpecHealth;
}SpmsSubSysHealth;

typedef union
{
   // SpareSubSysHealth stSpareHealth;
    SpmsSubSysHealth stSpmsHealth;
    unsigned char ucBuffer[1024];
} PeriodicHealthData;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;
    ObjDataHeader stObjDataHeader;
    PeriodicHealthData stPeriodicHealth;
    ObjDataTrailer stObjDataTrailer;
}HealthPacket;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;
    unsigned int uiComponentId;
    unsigned int uiComponentSubId;
   HardwareComponentType eComponentType;
} HmiToFdsHealthRequest;

typedef struct
{
    npluint32 ulOasMode;
    npluint32 ulTransmissionMode;
    npluint32 ulRangeScale;
    npluint32 ulPulseType;
    npluint32 ulPulseWidth;
    npluint32 ulSourceLevel;
    npluint32 ulTransmission;// Color Green iff OAS_XMN_ON
    npluint32 ulPresentationMode;
    npluint32 ulAudio;
    npluint32 ulClassifier;
    npluint32 ulZoom;
    nplfloat32 fZoomRange;
    npluint32 ulSensitivity;
    npluint32 ulHorizontalBeam;
    npluint32 ulVerticalBeam;
    npluint32 ulTestMode;
    npluint32 ulTxShading;
    npluint32 ulProcType;
    npluint32 ulShadingWindow;
    npluint32 ulGainMode;
    nplint32 lGainVal;
    npluint32 ulAGCRefLvl;
    npluint32 ulSpare[10];

}OasConfigType;

typedef struct
{
    ObjDataHeader stObjDataHeader;
    OasConfigType stOasConfig;
    ObjDataTrailer stObjDataTrailer;

}OasConfigObjectType;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;
    OasConfigObjectType stOasConfigObject;

}OasConfigPacketType;

typedef struct
{
    npluint32 ulPulseType;
    npluint32 ulSourceLevel;
    npluint32 ulPulseWidth;
    npluint32 ulXmnMode;
    npluint32 ulPingStatus;
    npluint32 ulSpare[3];
}ActiveTxParamType;

// Size: 32 bytes
typedef struct
{
    npluint32 ulVertAngle;
    npluint32 ulRangeScale;
    npluint32 ulZoomBm;
    npluint32 ulZoomPos;
    npluint32 ulSpare[4];
}ActiveRxParamType;

// Size: 64 bytes
typedef struct
{
    ActiveTxParamType stActiveTxParam;
    ActiveRxParamType stActiveRxParam;

}ActiveConfigType;

// Size: 80 bytes
typedef struct
{
    ObjDataHeader stObjDataHeader;
    ActiveConfigType stActiveConfig;
    ObjDataTrailer stObjDataTrailer;

}ActiveConfigObjectType;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;
    ActiveConfigObjectType stActiveConfigObject;

}ActiveConfigPacketType;



typedef struct
{

    int SONAR_Health_Value;
    int CICS_Health_Value;
    int NAV_Health_Value;
    int CTD_Health_VALUE;
    int OAS_Health_Value;
    int Active_Health_Value;
    int Ucs_Health_Value;
    int Sys_Mode_value;
    int CavitationThresholdFlag;
}SonarInterface;

typedef struct
{
    npluint32 ulUcsCommnMode;   //0x01: VOICE, 0x02: TEXT, 0x03: TELEGRAPH, 0x04: MESSAGE
    npluint32 ulUcsBand;        //0x01: EKM, 0x02: NATO
    npluint32 ulUcsXmnMode;     //0x00: OFF, 0x01: OMNI, 0x02: SECTOR
    npluint32 ulUcsXmnBeams[3];
    npluint32 ulUcsBaudRate;    //0x01: 8, 0x02: 16, 0x03: 32
    npluint32 ulPTTStatus;      //0x00: OFF, 0x01: ON
    npluint32 ulUcsRxMode;      //0x00: OFF, 0x01: NORMAL, 0x02: TEST
    npluint32 ulMsgTextValidity; //Flag indicating Msg or Text is valid. 0x00 INVALID 0x01 VALID
    npluint32 ulMsgNo;          //0x00 for INVALID
    npluchar8 ucText [UCS_DATA_LEN]; //NULL terminated string
    npluint32 ulSpare [53];
} UcsConfigType;

typedef struct
{
    ObjDataHeader stObjDataHeader;
    UcsConfigType stUcsConfig;
    ObjDataTrailer stObjDataTrailer;
} UcsConfigObjectType;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;            ///< Network header
    UcsConfigObjectType stUcsConfigObject;
} UcsConfigPacketType;

typedef struct
{
    npluint32 ulSystemMode;
    npluint32 ulTestPattern;
    npluint32 ulSpare[14];
} SystemModeConfigType;

typedef struct
{
    ObjDataHeader stObjDataHeader;
    SystemModeConfigType stSystemModeConfig;
    ObjDataTrailer stObjDataTrailer;
} SystemModeConfigObjectType;

typedef struct
{
    NetworkPacketHeaderType stNetHeader;            ///< Network header
    SystemModeConfigObjectType stSystemModeConfigObject;
} SystemModeConfigPacketType;

typedef struct
{
    char RecorderFilePath[1024];
    char RecorderPath[1024];
    char KELIMAGPath[400];
    char NPOLIMAGPath[400];
    char EnergyPath[400];
    char EnergyFilePath[400];
    char EnergyReplayFilePath[400];
    char CurrentDir[400];
    char ReplayFilePath[1024];

} RecordingControls;


typedef struct
{
    int32_t ChannelNo;
    int32_t SegmentNo;
    int32_t RecordDataBuffer1[16384];
} Recording;

//void swapBuffer(const char *data, unsigned long len);
//void updateInfoProcHardHealth(InfoProcHardHealth *pstInfoProcHardHealth);
//void updateInfoProcSoftHealth(InfoProcSoftHealth *pstInfoProcSoftHealth);
//bool getTemperature(int *iLocalTemp, int *iRemoteTemp);
//uint getBoardUpTimeInSec();
//uint getAppUpTimeInSec();


/* Software Designed and Developed by
 * Navil Paul,Asst Manager(STSG), KELTRON Controls,Kerala
 * Released Dated: 30/12/2017
 * Client: MTU Mumbai
 */

#endif //DATATYPE_H








