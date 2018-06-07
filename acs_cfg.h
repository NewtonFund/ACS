#define CFG_TYPE_STRING   0
#define CFG_TYPE_INT      1
#define CFG_TYPE_DOUBLE   2
#define CFG_TYPE_BOOLEAN  3
#define CFG_TYPE_LIST     4

#define CFG_SECT_GENERAL "General"
#define CFG_SECT_NET     "Network"
#define CFG_SECT_LIMIT   "Limits"
#define CFG_SECT_MAXERR  "MaxErrors"
#define CFG_SECT_TIMEOUT "Timeouts"
#define CFG_SECT_IERS    "IERS Data"
#define CFG_SECT_AG      "Autoguider"
#define CFG_SECT_MET     "Weather"
#define CFG_SECT_INST    "Instrument"


// Place holder variables for testing
  double acs_Latitude       ; // Decimal
  double acs_Longitude      ; // Decimal
  double acs_Altitude       ; // Decimal
                             
  char   acs_IPType[99]     ; // String 
  int    acs_IPPort         ; // Integer
                           
  char   acs_Dir            ; // String 
                            
  double acs_LimitDecHi     ; // Decimal
  double acs_LimitDecLo     ; // Decimal
  double acs_LimitZDHi      ; // Decimal
  double acs_LimitZDlo      ; // Decimal
  double acs_LimitAzmHi     ; // Decimal
  double acs_LimitAzmLo     ; // Decimal
  double acs_LimitRotatorHi ; // Decimal
  double acs_LimitRotatorLo ; // Decimal
                           
  double acs_MaxErrZD       ; // Decimal
  double acs_MaxErrAzm      ; // Decimal
  double acs_MaxErrRotator  ; // Decimal
                           
  double acs_TimeoutSlew    ; // Decimal
  double acs_TimeoutAG      ; // Decimal
  double acs_TimeoutMet     ; // Decimal
  double acs_TimeoutAxis    ; // Decimal
  double acs_TimeoutReply   ; // Decimal
  double acs_TimeoutMessage ; // Decimal
                           
  double acs_LeapSecs       ; // Decimal
  double acs_DUTC           ; // Decimal
  double acs_PolarX         ; // Decimal
  double acs_PolarY         ; // Decimal
                            
  char   acs_MessageIP[99]  ; // String 
  char   acs_DemandIP[99]   ; // String 
  int    acs_DemandFreq     ; // Integer
                           
  char    acs_AG1IP[99]     ; // String 
  char    acs_AG1Frame[99]  ; // String 
  char    acs_AG1Type[99]   ; // String 
  double  acs_AG1RotAngle   ; // Decimal
  double  acs_AG1Wavelength ; // Decimal
                           
  char    acs_AGSmoothing[99]; // String 
  int     acs_AGSampleSize   ; // Integer
                          
  char    acs_MetIP[99]     ; // String 
  double  acs_MetPress      ; // Decimal
  double  acs_MetTemp       ; // Decimal
  double  acs_MetRH         ; // Decimal
  double  acs_Wavelength    ; // Decimal
  char    acs_Smoothing     ; // String 
  int     acs_SampleSize    ; // Integer
                           
 char     acs_InstName[99]  ; // String 
 double   acs_InstRotAngle  ; // Decimal
 double   acs_Wavelength    ; // Decimal


struct acs_config_t {
    char *key;  // Key name
    char *sect; // Section name
    int   type; // Data type
    void *ptr;  // Pointer to variable
//  void *dflt; // Default value
//  double lo;  // Sanity check 
//  double hi;  // Sanity check
} acs_config[] = 
{ 
//                                                                             // Type      Units     Description
  { "Latitude",       CFG_SECT_GENERAL, CFG_TYPE_DOUBLE, &acs_Latitude      }, // Decimal   degrees   ITRF Latitude               
  { "Longitude",      CFG_SECT_GENERAL, CFG_TYPE_DOUBLE, &acs_Longitude     }, // Decimal   degrees   ITRF Longitude
  { "Altitude",       CFG_SECT_GENERAL, CFG_TYPE_DOUBLE, &acs_Altitude      }, // Decimal   m         Height above WGS84 geoid

  { "IPType",         CFG_SECT_NET,     CFG_TYPE_STRING, &acs_IPType        }, // String    [TCP,UDP] Network protocol
  { "IPPort",         CFG_SECT_NET,     CFG_TYPE_INT,    &acs_IPPort        }, // Integer             Network receive port
                                                                                      
  { "ACSDir",         CFG_SECT_GENERAL, CFG_TYPE_STRING, &acs_Dir           }, // String              ACS working directory       
                                                                                      
  { "LimitDecHi",     CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitDecHi    }, // Decimal   degrees   Declination high limit      
  { "LimitDecLo",     CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitDecLo    }, // Decimal   degrees   Declination low limit       
  { "LimitZDHi",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitZDHi     }, // Decimal   degrees   Zenith distance high limit  
  { "LimitZDLo",      CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitZDlo     }, // Decimal   degrees   Zenith distance low limit   
  { "LimitAzmHi",     CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitAzmHi    }, // Decimal   degrees   Azimuth angle high limit    
  { "LimitAzmLo",     CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitAzmLo    }, // Decimal   degrees   Azimuth angle low limit     
  { "LimitRotatorHi", CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitRotatorHi}, // Decimal   degrees   Rotator angle high limit    
  { "LimitRotatorLo", CFG_SECT_LIMIT,   CFG_TYPE_DOUBLE, &acs_LimitRotatorLo}, // Decimal   degrees   Rotator angle low limit     
                                                                                     
  { "MaxErrZD",       CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrZD      }, // Decimal   mas        Max. tracking error zenith distance
  { "MaxErrAzm",      CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrAzm     }, // Decimal   mas        Max. tracking error azimuth angle  
  { "MaxErrRotator",  CFG_SECT_MAXERR,  CFG_TYPE_DOUBLE, &acs_MaxErrRotator }, // Decimal   mas        Max. tracking error rotator        
                                                                                        
  { "TimeoutSlew",    CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutSlew   }, // Decimal   s          Timeout for a slew              
  { "TimeoutAG",      CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutAG     }, // Decimal   s          Timeout for AG message          
  { "TimeoutMet",     CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutMet    }, // Decimal   s          Timeout for weather message     
  { "TimeoutAxis",    CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutAxis   }, // Decimal   s          Timeout for axis state message  
  { "TimeoutReply",   CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutReply  }, // Decimal   s          Timeout for acknowledgment      
  { "TimeoutMessage", CFG_SECT_TIMEOUT, CFG_TYPE_DOUBLE, &acs_TimeoutMessage}, // Decimal   s          Rejection age for a message     
                                                                                      
  { "LeapSecs",       CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_LeapSecs      }, // Decimal   mas/year   IERS leap seconds            
  { "DUTC",           CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_DUTC          }, // Decimal   mas        IERS Delta UTC               
  { "PolarX",         CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_PolarX        }, // Decimal   mas        IERS Polar X drift           
  { "PolarY",         CFG_SECT_IERS,    CFG_TYPE_DOUBLE, &acs_PolarY        }, // Decimal   mas        IERS Polar Y drift           
                                                                                         
  { "MessageIP",      CFG_SECT_NET,     CFG_TYPE_STRING, &acs_MessageIP     }, // String    IP address:port              Default destination for outbound message
  { "DemandIP",       CFG_SECT_NET,     CFG_TYPE_STRING, &acs_DemandIP      }, // String    IP address:port              Destination for output tracking demands  
  { "DemandFreq",     CFG_SECT_GENERAL, CFG_TYPE_INT,    &acs_DemandFreq    }, // Integer   Hz                           Tracking demand send rate    
                                                                                        
  { "AGnIP",          CFG_SECT_AG,      CFG_TYPE_STRING, &acs_AG1IP         }, // String    IP address:port              Autoguider sources n=0,1,2...
  { "AGnFrame",       CFG_SECT_AG,      CFG_TYPE_STRING, &acs_AG1Frame      }, // String    INSTRUMENT CELESTIAL, MOUNT  Reference frame for offsets     
  { "AGnType",        CFG_SECT_AG,      CFG_TYPE_STRING, &acs_AG1Type       }, // String    STAR,  WCS                   Type of AG guiding              
  { "AGnRotAngle",    CFG_SECT_AG,      CFG_TYPE_DOUBLE, &acs_AG1RotAngle   }, // Decimal   degrees                      Angular location of AG field within rotator  
  { "AGnWavelength",  CFG_SECT_AG,      CFG_TYPE_DOUBLE, &acs_AG1Wavelength }, // Decimal   nm                           AG wavelength                                    
  { "AGSmoothing",    CFG_SECT_AG,      CFG_TYPE_STRING, &acs_AGSmoothing   }, // String                                 AG smoothing algorithm       
  { "AGSampleSize",   CFG_SECT_AG,      CFG_TYPE_INT,    &acs_AGSampleSize  }, // Integer                                AG smoothing sample size     
                                                                                       
  { "MetIP",          CFG_SECT_MET,     CFG_TYPE_STRING, &acs_MetIP         }, // String    IP address:port              Weather station network address
  { "MetPress",       CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetPress      }, // Decimal   mBar = hPa                   Default atmospheric pressure 
  { "MetTemp",        CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetTemp       }, // Decimal   C                            Default external temperature 
  { "MetRH",          CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_MetRH         }, // Decimal   Range 0.0 -- 1.0             Default relative humidity    
  { "MetWavelength",  CFG_SECT_MET,     CFG_TYPE_DOUBLE, &acs_Wavelength    }, // Decimal   nm                           Default observing wavelength 
  { "MetSmoothing",   CFG_SECT_MET,     CFG_TYPE_STRING, &acs_Smoothing     }, // String                                 Met. smoothing algorithm     
  { "MetSampleSize",  CFG_SECT_MET,     CFG_TYPE_INT,    &acs_SampleSize    }, // Integer                                Met. smoothing sample size   
                                                                                       
  { "InstName",       CFG_SECT_INST,    CFG_TYPE_STRING, &acs_InstName      }, // String    Instrument ID                Default instrument           
  { "InstRotAngle",   CFG_SECT_INST,    CFG_TYPE_DOUBLE, &acs_InstRotAngle  }, // Decimal   degrees [-180 -- +180]       Angular offset of instrument from its rotator zero      
  { "InstWavelength", CFG_SECT_INST,    CFG_TYPE_DOUBLE, &acs_Wavelength    }  // Decimal   nm                           Wavelength for refraction correction of target position
};

#define acs_config_siz ( sizeof( acs_config ) / sizeof( acs_config[0] ) )

