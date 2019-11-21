/*!
 * Look-up tables and associated #defines 
 * For the main() module the global data is both defined and initialised here  
 * For all other modules the data declarated as extern in a block at the bottom of this file 
 */

/*
** CONFIGURATION:
** Configuration information is read from a .ini file into global variables  
*/

// Data types that can be read from .ini file
#define CFG_TYPE_STRING   0
#define CFG_TYPE_INT      1
#define CFG_TYPE_DOUBLE   2
#define CFG_TYPE_BOOLEAN  3
#define CFG_TYPE_LIST     4

// Section names within .ini file
#define CFG_SECT_GENERAL "General"
#define CFG_SECT_NET     "Network"
#define CFG_SECT_LIMIT   "Limits"
#define CFG_SECT_MAXERR  "MaxErrors"
#define CFG_SECT_TIMEOUT "Timeouts"
#define CFG_SECT_IERS    "IERS Data"
#define CFG_SECT_AG      "Autoguider"
#define CFG_SECT_MET     "Weather"
#define CFG_SECT_INST    "Instrument"

#ifdef ACS_MAIN
// Table defning the configuration data that can be read from .ini file
acs_cfg_t acs_cfg[] = {
// Field text        Section ID        Field Type       Pointer to storage     // Type    Units     Description
{ "ACSDir",          CFG_SECT_GENERAL, CFG_TYPE_STRING, &acs_Dir            }, // String            ACS working directory       
{ "Latitude",        CFG_SECT_GENERAL, CFG_TYPE_STRING, &str_Lat            }, // String  degrees   ITRF Latitude               
{ "Longitude",       CFG_SECT_GENERAL, CFG_TYPE_STRING, &str_Lon            }, // String  degrees   ITRF Longitude
{ "Altitude",        CFG_SECT_GENERAL, CFG_TYPE_DOUBLE, &acs_Alt            }, // Decimal m         Height above WGS84 geoid
{ "DemandFreq",      CFG_SECT_GENERAL, CFG_TYPE_INT,    &acs_DemandFreq     }, // Integer Hz        Tracking demand send rate    

{ "Protocol",        CFG_SECT_NET,     CFG_TYPE_STRING, &str_Protocol       }, // String  IP [TCP,UDP]    Network protocol
{ "ACSAddrPort",     CFG_SECT_NET,     CFG_TYPE_STRING, &str_ACSAddrPort    }, // String  IP address:port ACS 
{ "CommandAddrPort", CFG_SECT_NET,     CFG_TYPE_STRING, &str_CommandAddrPort}, // String  IP address:port Source of commands 
{ "DemandAddrPort",  CFG_SECT_NET,     CFG_TYPE_STRING, &str_DemandAddrPort }, // String  IP address:port Destination for demands

{ "LimitZDHi",       CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitZDHi      }, // Decimal degrees    Zenith distance high limit  
{ "LimitZDLo",       CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitZDlo      }, // Decimal degrees    Zenith distance low limit   
{ "LimitAzmHi",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitAzmHi     }, // Decimal degrees    Azimuth angle high limit    
{ "LimitAzmLo",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitAzmLo     }, // Decimal degrees    Azimuth angle low limit     
{ "LimitRotHi",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitRotHi     }, // Decimal degrees    Rotator angle high limit    
{ "LimitRotLo",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitRotLo     }, // Decimal degrees    Rotator angle low limit     

{ "MaxErrZD",        CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrZD       }, // Decimal mas        Max. tracking error zenith distance
{ "MaxErrAzm",       CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrAzm      }, // Decimal mas        Max. tracking error azimuth angle  
{ "MaxErrRot",       CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrRot      }, // Decimal mas        Max. tracking error rotator        

{ "TimeoutSlew",     CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutSlew    }, // Decimal s          Timeout for a slew              
{ "TimeoutAG",       CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutAG      }, // Decimal s          Timeout for AG message          
{ "TimeoutMet",      CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutMet     }, // Decimal s          Timeout for weather message     
{ "TimeoutAxis",     CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutAxis    }, // Decimal s          Timeout for axis state message  
{ "TimeoutReply",    CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutReply   }, // Decimal s          Timeout for acknowledgment      
{ "TimeoutAge"    ,  CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutAge     }, // Decimal s          Rejection age for a message     

{ "LeapSecs",        CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_LeapSecs       }, // Decimal mas/year   IERS leap seconds            
{ "DUTC",            CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_DUTC           }, // Decimal mas        IERS Delta UTC               
{ "PolarX",          CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_PolarX         }, // Decimal mas        IERS Polar X drift           
{ "PolarY",          CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_PolarY         }, // Decimal mas        IERS Polar Y drift           

{ "AGAddrPort",      CFG_SECT_AG,      CFG_TYPE_STRING, &str_AGAddrPort     }, // String  IP address:port              Autoguider sources n=0,1,2...
{ "AGFrame",         CFG_SECT_AG,      CFG_TYPE_STRING, &str_AGFrame        }, // String  INSTRUMENT,CELESTIAL,MOUNT   Reference frame for offsets     
{ "AGType",          CFG_SECT_AG,      CFG_TYPE_STRING, &str_AGType         }, // String  STAR,  WCS                   Type of AG guiding              
{ "AGRotAngle",      CFG_SECT_AG,      CFG_TYPE_DOUBLE, &acs_AGRotAngle     }, // Decimal degrees                      Angular location of AG field within rotator  
{ "AGWavelength",    CFG_SECT_AG,      CFG_TYPE_DOUBLE, &acs_AGWavelength   }, // Decimal nm                           AG wavelength                                    
{ "AGSmoothing",     CFG_SECT_AG,      CFG_TYPE_STRING, &acs_AGSmoothing    }, // String                               AG smoothing algorithm       
{ "AGSampleSize",    CFG_SECT_AG,      CFG_TYPE_INT,    &acs_AGSampleSize   }, // Integer                              AG smoothing sample size     

{ "MetAddrPort",     CFG_SECT_MET,     CFG_TYPE_STRING, &str_MetAddrPort    }, // String  IP address                   Weather station network address
{ "MetPress",        CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetPress       }, // Decimal mBar = hPa                   Default atmospheric pressure 
{ "MetTemp",         CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetTemp        }, // Decimal C                            Default external temperature 
{ "MetRH",           CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetRH          }, // Decimal Range 0.0 -- 1.0             Default relative humidity    
{ "MetWavelength",   CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetWavelength  }, // Decimal nm                           Default observing wavelength 
{ "MetSmoothing",    CFG_SECT_MET,     CFG_TYPE_STRING, &acs_MetSmoothing   }, // String                                 Met. smoothing algorithm     
{ "MetSampleSize",   CFG_SECT_MET,     CFG_TYPE_INT,    &acs_MetSampleSize  }, // Integer                                Met. smoothing sample size   

{ "InstName",        CFG_SECT_INST,    CFG_TYPE_STRING, &acs_InstName       }, // String  Instrument ID                Default instrument           
{ "InstRotAngle",    CFG_SECT_INST,    CFG_TYPE_DOUBLE, &acs_InstRotAngle   }, // Decimal degrees [-180 -- +180]       Angular offset of instrument from its rotator zero      
{ "InstWavelength",  CFG_SECT_INST,    CFG_TYPE_DOUBLE, &acs_InstWavelength }};// Decimal nm                           Wavelength for refraction correction of target position
size_t const acs_cfg_siz = sizeof(acs_cfg_t);
size_t const acs_cfg_num = sizeof(acs_cfg) / sizeof(acs_cfg_t);
#endif

#ifdef ACS_MAIN
acs_cmd_t acs_cmds[] = { 
/*
** COMMANDS:
** All recognised commands are defined in this table
*/
//Command name         Function             Type    
{ ACS_CMD_TRACKINIT  , acs_cmd_TrackInit  , ACS_DIR_INBOUND  }, // Tracking initialise     
{ ACS_CMD_TRACKCORR  , acs_cmd_TrackCorr  , ACS_DIR_INBOUND  }, // Tracking corrections    
{ ACS_CMD_TRACKOFFSET, acs_cmd_TrackOffset, ACS_DIR_INBOUND  }, // Tracking offsets        
{ ACS_CMD_TRACKENABLE, acs_cmd_TrackEnable, ACS_DIR_INBOUND  }, // Tracking control        
{ ACS_CMD_TRACKEND   , acs_cmd_TrackEnd   , ACS_DIR_INBOUND  }, // Tracking end            
{ ACS_CMD_TRACKWRITE , acs_cmd_TrackWrite , ACS_DIR_INBOUND  }, // Write tracking info       Engineering
{ ACS_CMD_TRACKREAD  , acs_cmd_TrackRead  , ACS_DIR_INBOUND  }, // Read tracking info        Engineering
{ ACS_CMD_TRACKRADEC , acs_cmd_TrackRADec , ACS_DIR_INBOUND  }, // Tracking to RA/DEC      
{ ACS_CMD_AGCORR     , acs_cmd_AGCorr     , ACS_DIR_INBOUND  }, // Autoguider correction   
{ ACS_CMD_AGDATUM    , acs_cmd_AGDatum    , ACS_DIR_INBOUND  }, // Autoguider datum        
{ ACS_CMD_METINFO    , acs_cmd_MetInfo    , ACS_DIR_INBOUND  }, // Meteorology information 
{ ACS_CMD_INSTINFO   , acs_cmd_InstInfo   , ACS_DIR_INBOUND  }, // Instrument information  
{ ACS_CMD_AXISINFO   , acs_cmd_AxisInfo   , ACS_DIR_INBOUND  }, // Axis information        
{ ACS_CMD_AXISDEMAND , acs_cmd_AxisDemand , ACS_DIR_OUTBOUND }, // Axis demand             
{ ACS_CMD_ACSGETPARAM, acs_cmd_ACSGetParam, ACS_DIR_INBOUND  }, // Get parameter           
{ ACS_CMD_ACSSETPRARM, acs_cmd_ACSSetParam, ACS_DIR_INBOUND  }, // Set parameter           
{ ACS_CMD_ACSRECONFIG, acs_cmd_ACSReconfig, ACS_DIR_INBOUND  }, // Re-read configuration   
{ ACS_CMD_ACSCHANGE  , acs_cmd_ACSChange  , ACS_DIR_OUTBOUND }, // State change            
{ ACS_CMD_ACSREPLY   , acs_cmd_ACSReply   , ACS_DIR_OUTBOUND }};// Reply                   
size_t const acs_cmds_siz = sizeof(acs_cmd_t);
size_t const acs_cmds_num = sizeof(acs_cmds) / sizeof(acs_cmd_t);

/*
** Text values are converted to pseudo eunumerations using these tables
** Zero value is reserved to represent a failed enum lookup
** End of each table is marked with by a nul
*/ 
acs_enum_t acs_enum_equinox[] = {
{ ACS_EQUINOX_B1950_0_TXT, ACS_EQUINOX_B1950_0}, 
{ ACS_EQUINOX_J2000_0_TXT, ACS_EQUINOX_J2000_0}, 
{ ACS_EQUINOX_ICRS_TXT   , ACS_EQUINOX_ICRS   },
{ ACS_EQUINOX_TIME_TXT   , ACS_EQUINOX_TIME   },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }}; 

acs_enum_t acs_enum_frame[] = {
{ ACS_FRAME_CELEST_TXT   , ACS_FRAME_CELEST   },
{ ACS_FRAME_MOUNT_TXT    , ACS_FRAME_MOUNT    },
{ ACS_FRAME_INST_TXT     , ACS_FRAME_INST     },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};

acs_enum_t acs_enum_rotframe[] = {
{ ACS_ROTFRAME_CELEST_TXT, ACS_ROTFRAME_CELEST},
{ ACS_ROTFRAME_MOUNT_TXT , ACS_ROTFRAME_MOUNT },
{ ACS_ROTFRAME_PARAL_TXT , ACS_ROTFRAME_PARAL },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};

// Types of correction  or offsets
acs_enum_t acs_enum_type[] = {
{ ACS_TYPE_FIXED_TXT     , ACS_TYPE_FIXED },
{ ACS_TYPE_CUMUL_TXT     , ACS_TYPE_CUMUL }, 
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};

acs_enum_t acs_enum_ag[] = {
{ ACS_AG_STAR_TXT        , ACS_AG_STAR        }, 
{ ACS_AG_WCS_TXT         , ACS_AG_WCS         },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};

acs_enum_t acs_enum_action[] = {
{ ACS_LOGACTION_REPLAY_TXT, ACS_LOGACTION_REPLAY},
{ ACS_LOGACTION_GOTO_TXT  , ACS_LOGACTION_GOTO  },
{ ACS_EMPTY_TXT           , ACS_ENUM_NOT_FOUND  }};

acs_enum_t acs_enum_smoothing[] = {
{ ACS_SMOOTHING_NONE_TXT , ACS_SMOOTHING_NONE },
{ ACS_SMOOTHING_MEAN_TXT , ACS_SMOOTHING_MEAN },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};

acs_enum_t acs_enum_axis_state[] = {
{ ACS_STATE_IDLE_TXT     , ACS_STATE_IDLE     },
{ ACS_STATE_SLEW_TXT     , ACS_STATE_SLEW     },
{ ACS_STATE_TRACK_TXT    , ACS_STATE_TRACK    },
{ ACS_STATE_WARN_TXT     , ACS_STATE_WARN     },
{ ACS_STATE_ERROR_TXT    , ACS_STATE_ERROR    },
{ ACS_EMPTY_TXT          , ACS_ENUM_NOT_FOUND }};


/*
** TAGS:
** All recognised tags within messages are defined by this table.
** Before use, the table is sorted with qsort() so it can be searched using bsearch() 
** Parameter is type checked by calling a Check fn(), NULL suppresses the check.
** Similarly, Validate functions perform context/command specific checks 
**
** The Min and Max limits have different meaning according to data type
**  ACS_TYP_DBL:  Numerical value limits
**  ACS_TYP_INT:  Numerical value limits
**  ACS_TYP_BOOL: true or false 
**  ACS_TYP_ENUM: Enumeration limits are as defined in look-up tables in acs_tbl.h
**  ACS_TYP_STR:  String length limits
**  ACS_TYP_IP:   Unused
**  ASC_TYP_TIME: Unused
*/
acs_tag_t acs_tags[] = { 
//Tag name                Tag type      Variable address     Check fn()        Min     Max  Validate fn()
{ ACS_TAG_TRACKEQUINOX  , ACS_TYP_ENUM, &acs_TrackEquinox  , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_TRACKEPOCH    , ACS_TYP_TIME, &acs_TrackEpoch    , acs_chk_time,     0.0,    0.0, NULL },
{ ACS_TAG_TRACKRA       , ACS_TYP_DBL , &acs_TrackRA       , acs_chk_dbl ,-DBL_MAX,   24.0, NULL },
{ ACS_TAG_TRACKDEC      , ACS_TYP_DBL , &acs_TrackDec      , acs_chk_dbl ,   -90.0,   90.0, NULL },
{ ACS_TAG_TRACKPMRA     , ACS_TYP_DBL , &acs_TrackPMRA     , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_TRACKPMDEC    , ACS_TYP_DBL , &acs_TrackPMDec    , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_TRACKPARALLAX , ACS_TYP_DBL , &acs_TrackParallax , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_TRACKRADVEL   , ACS_TYP_DBL , &acs_TrackRadVel   , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_TRACKROTFRAME , ACS_TYP_ENUM, &acs_TrackRotFrame , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_TRACKROTANGLE , ACS_TYP_DBL , &acs_TrackRotAngle , acs_chk_dbl ,  -180.0,  180.0, NULL },
{ ACS_TAG_TRACKFREQ     , ACS_TYP_DBL , &acs_TrackFreq     , acs_chk_dbl ,    -1.0,  400.0, NULL },
{ ACS_TAG_DEMANDADDRPORT, ACS_TYP_IP  , &str_DemandAddrPort, acs_chk_ip  ,     0.0,    0.0, NULL },
{ ACS_TAG_TRACKSTART    , ACS_TYP_TIME, &acs_TrackStart    , acs_chk_time,     0.0,    0.0, NULL },
{ ACS_TAG_TRACKDURATION , ACS_TYP_DBL , &acs_TrackDuration , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_ENABLEAG      , ACS_TYP_BOOL, &acs_EnableAG      , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEMET     , ACS_TYP_BOOL, &acs_EnableMet     , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLECORR    , ACS_TYP_BOOL, &acs_EnableCorr    , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEOFFSET  , ACS_TYP_BOOL, &acs_EnableOffset  , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEPMRA    , ACS_TYP_BOOL, &acs_EnablePMRA    , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEPMDEC   , ACS_TYP_BOOL, &acs_EnablePMDec   , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEZD      , ACS_TYP_BOOL, &acs_EnableZD      , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEAZM     , ACS_TYP_BOOL, &acs_EnableAzm     , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_ENABLEROT     , ACS_TYP_BOOL, &acs_EnableRot     , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_CORRTYPE      , ACS_TYP_ENUM, &acs_CorrType      , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_CORRFRAME     , ACS_TYP_ENUM, &acs_CorrFrame     , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_CORRX         , ACS_TYP_DBL , &acs_CorrX         , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_CORRY         , ACS_TYP_DBL , &acs_CorrY         , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_CORRROTANGLE  , ACS_TYP_DBL , &acs_CorrRot       , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_OFFSETTYPE    , ACS_TYP_ENUM, &acs_OffsetType    , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_OFFSETFRAME   , ACS_TYP_ENUM, &acs_OffsetFrame   , acs_chk_enum,       1,      0, NULL },
{ ACS_TAG_OFFSETX       , ACS_TYP_DBL , &acs_OffsetX       , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_OFFSETY       , ACS_TYP_DBL , &acs_OffsetY       , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_OFFSETROTANGLE, ACS_TYP_DBL , &acs_OffsetRot     , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AGTYPE        , ACS_TYP_ENUM, &acs_AGType        , acs_chk_str ,       1,      0, NULL },
{ ACS_TAG_AGFRAME       , ACS_TYP_ENUM, &acs_AGFrame       , acs_chk_str ,       1,      0, NULL },
{ ACS_TAG_AGX           , ACS_TYP_DBL , &acs_AGX           , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AGY           , ACS_TYP_DBL , &acs_AGY           , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AGROTANGLE    , ACS_TYP_DBL , &acs_AGRotAngle    , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AGROTATES     , ACS_TYP_BOOL, &acs_AGRotates     , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_AGWAVELENGTH  , ACS_TYP_DBL , &acs_AGWavelength  , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_METTEMP       , ACS_TYP_DBL , &acs_MetTemp       , acs_chk_dbl ,  -273.0,DBL_MAX, NULL },
{ ACS_TAG_METPRESS      , ACS_TYP_DBL , &acs_MetPress      , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_METRH         , ACS_TYP_DBL , &acs_MetRH         , acs_chk_dbl ,     0.0,    1.0, NULL },
{ ACS_TAG_INSTNAME      , ACS_TYP_STR , &acs_InstName      , acs_chk_str ,       1,STR_MAX, NULL },
{ ACS_TAG_INSTROTANGEL  , ACS_TYP_DBL , &acs_InstRotAngle  , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_INSTROTATES   , ACS_TYP_BOOL, &acs_InstRotates   , acs_chk_bool,   false,   true, NULL },
{ ACS_TAG_INSTWAVELENGTH, ACS_TYP_DBL , &acs_InstWavelength, acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AXISSTATE     , ACS_TYP_INT , &acs_AxisState     , acs_chk_int , ACS_STATE_IDLE, ACS_STATE_ERROR, NULL },
{ ACS_TAG_AXISZD        , ACS_TYP_DBL , &acs_AxisZD        , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AXISAZM       , ACS_TYP_DBL , &acs_AxisAzm       , acs_chk_dbl ,-DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_AXISROTANGLE  , ACS_TYP_DBL , &acs_AxisRotAngle  , acs_chk_dbl, -DBL_MAX,DBL_MAX, NULL },
{ ACS_TAG_LOGFILE       , ACS_TYP_STR , &acs_LogFile       , acs_chk_str ,     0.0,STR_MAX, NULL },
{ ACS_TAG_LOGADDRPORT   , ACS_TYP_IP  , &str_LogAddrPort   , acs_chk_ip  ,     0.0,    0.0, NULL },
{ ACS_TAG_LOGFREQ       , ACS_TYP_DBL , &acs_LogFreq       , acs_chk_dbl ,    -1.0,  400.0, NULL },
{ ACS_TAG_LOGACTION     , ACS_TYP_ENUM, &acs_LogAction     , acs_chk_str ,       1,      0, NULL },
{ ACS_TAG_ACSNEWSTATE   , ACS_TYP_STR , &acs_ACSNewState   , acs_chk_str ,       1,STR_MAX, NULL },
{ ACS_TAG_ACSREASON     , ACS_TYP_STR , &acs_ACSReason     , acs_chk_str ,       1,STR_MAX, NULL }};
size_t const acs_tags_siz = sizeof(acs_tag_t);
size_t const acs_tags_num = sizeof(acs_tags) / sizeof(acs_tag_t);
#endif


/*
** If not main() then declare data as external 
*/
#ifndef ACS_MAIN
extern acs_cfg_t    acs_cfg[];
extern const size_t acs_cfg_siz;
extern const size_t acs_cfg_num;

extern acs_cmd_t    acs_cmds[]; 
extern const size_t acs_cmds_siz;
extern const size_t acs_cmds_num;

extern acs_tag_t    acs_tags[]; 
extern const size_t acs_tags_siz;
extern const size_t acs_tags_num;

extern acs_enum_t acs_enum_equinox[];
extern acs_enum_t acs_enum_frame[];
extern acs_enum_t acs_enum_rotframe[];
extern acs_enum_t acs_enum_type[];
extern acs_enum_t acs_enum_ag[];
extern acs_enum_t acs_enum_action[];
extern acs_enum_t acs_enum_smoothing[];
#endif
