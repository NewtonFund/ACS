/*
 * Global data storage
 * acs_ prefix are working values used by the astrometric engine sub-process
 * str_ prefix indicates stored strings that are converted to a pseudo-enumeration (Enum) 
 *
 */


// System global data declared as EXTERN for all other modules
EXTERN int        acs_dbg_lvl             ;  // Debug level
EXTERN pid_t      acs_cmdpid              ;  // Command process PID
EXTERN pid_t      acs_dmdpid              ;  // nemand  process PID
EXTERN acs_mem_t *acs_mem                 ;  // Share memory between processes
EXTERN acs_net_t  acs_net_dmd;            ; 
EXTERN acs_net_t  acs_net_cmd;            ; 

/*
** 
** Global data within a process shared between all compilation modules
** Values are modified in this order:
**    0 - Default values set when data is declared 
**    1 - Configuration values from .ini file 
**    2 - Incoming command message
**    3 - Internal process updates/changes 
*/
EXTERN char   str_Lat[STR_MAX]            ;
EXTERN char   str_Lon[STR_MAX]            ;
EXTERN double acs_Alt                     ;

EXTERN char   str_Protocol[STR_MAX]       ;
EXTERN enum   PSocketProtocol_ acs_sktProtocol;
EXTERN char   str_AddrPort[STR_MAX]       ;
EXTERN char   acs_Addr[STR_MAX]           ;
EXTERN int    acs_Port                    ;
                                          
EXTERN char   acs_Dir[DIR_MAX]            ;

EXTERN double acs_LimitZDHi               ;
EXTERN double acs_LimitZDlo               ;
EXTERN double acs_LimitAzmHi              ;
EXTERN double acs_LimitAzmLo              ;
EXTERN double acs_LimitRotHi              ;
EXTERN double acs_LimitRotLo              ;

EXTERN double acs_MaxErrZD                ;
EXTERN double acs_MaxErrAzm               ;
EXTERN double acs_MaxErrRot               ;

// -ve timeout = infinite                 
EXTERN double acs_TimeoutSlew             ; 
EXTERN double acs_TimeoutAG               ;
EXTERN double acs_TimeoutMet              ;
EXTERN double acs_TimeoutAxis             ;
EXTERN double acs_TimeoutReply            ; 
EXTERN double acs_TimeoutAge              ;

EXTERN double acs_LeapSecs                ;
EXTERN double acs_DUTC                    ;
EXTERN double acs_PolarX                  ;
EXTERN double acs_PolarY                  ;

EXTERN char   str_CommandAddrPort[STR_MAX];
EXTERN char   acs_CommandAddr[STR_MAX]    ;
EXTERN int    acs_CommandPort             ;
EXTERN char   str_DemandAddrPort[STR_MAX] ;
EXTERN char   acs_DemandAddr[STR_MAX]     ;
EXTERN int    acs_DemandPort              ;
EXTERN int    acs_DemandFreq              ;
EXTERN struct timespec acs_DemandInterval ; // Time between demands 
EXTERN struct timespec acs_DemandLatency  ; // Time to evaluate a demand 

EXTERN char   str_AGAddrPort[STR_MAX]     ;
EXTERN char   acs_AGAddr[STR_MAX]         ;
EXTERN int    acs_AGPort                  ;
EXTERN char   str_AGFrame[STR_MAX]        ;
EXTERN int    acs_AGFrame                 ;
EXTERN char   str_AGType[STR_MAX]         ;
EXTERN int    acs_AGType                  ;
EXTERN bool   acs_AGRotates               ;
EXTERN double acs_AGX                     ;
EXTERN double acs_AGY                     ;
EXTERN double acs_AGRotAngle              ;
EXTERN double acs_AGWavelength            ;
EXTERN char   str_AGSmoothing[STR_MAX]    ;
EXTERN int    acs_AGSmoothing             ;
EXTERN int    acs_AGSampleSize            ;

EXTERN char   str_MetAddrPort[STR_MAX]    ;
EXTERN char   acs_MetAddr[STR_MAX]        ;
EXTERN int    acs_MetPort                 ;
EXTERN double acs_MetPress                ;
EXTERN double acs_MetTemp                 ;
EXTERN double acs_MetRH                   ;
EXTERN double acs_MetWavelength           ;
EXTERN char   str_MetSmoothing[STR_MAX]   ;  
EXTERN int    acs_MetSmoothing            ;
EXTERN int    acs_MetSampleSize           ;

EXTERN char   acs_InstName[STR_MAX]       ;       
EXTERN double acs_InstRotAngle            ;
EXTERN double acs_InstWavelength          ;
EXTERN bool   acs_InstRotates             ;

EXTERN int    acs_TrackEquinox            ;
EXTERN acs_time_t acs_TrackEpoch          ; 
EXTERN double acs_TrackRA                 ;
EXTERN double acs_TrackDec                ;
EXTERN double acs_TrackPMRA               ;
EXTERN double acs_TrackPMDec              ;
EXTERN double acs_TrackParallax           ;
EXTERN double acs_TrackRadVel             ;
EXTERN char   str_TrackRotFrame[STR_MAX]  ;
EXTERN int    acs_TrackRotFrame           ;
EXTERN double acs_TrackRotAngle           ;
EXTERN double acs_TrackFreq               ;
EXTERN acs_time_t acs_TrackStart          ;
EXTERN double acs_TrackDuration           ;

EXTERN bool   acs_EnableAG                ;
EXTERN bool   acs_EnableMet               ;
EXTERN bool   acs_EnableCorr              ;
EXTERN bool   acs_EnableOffset            ;
EXTERN bool   acs_EnablePMRA              ;
EXTERN bool   acs_EnablePMDec             ;
EXTERN bool   acs_EnableZD                ;
EXTERN bool   acs_EnableAzm               ;
EXTERN bool   acs_EnableRot               ;

EXTERN char   str_CorrType[STR_MAX]       ;
EXTERN int    acs_CorrType                ;
EXTERN char   str_CorrFrame[STR_MAX]      ;
EXTERN int    acs_CorrFrame               ;
EXTERN double acs_CorrX                   ;
EXTERN double acs_CorrY                   ;
EXTERN double acs_CorrRotAngle            ;

EXTERN char   str_OffsetType[STR_MAX]     ;
EXTERN int    acs_OffsetType              ;
EXTERN char   str_OffsetFrame[STR_MAX]    ;
EXTERN int    acs_OffsetFrame             ;
EXTERN double acs_OffsetX                 ;
EXTERN double acs_OffsetY                 ;
EXTERN double acs_OffsetRotAngle          ;

EXTERN int    acs_AxisState               ;
EXTERN double acs_AxisZD                  ;
EXTERN double acs_AxisAzm                 ;
EXTERN double acs_AxisRotAngle            ;

EXTERN char   acs_LogFile[STR_MAX]        ;
EXTERN char   str_LogAddrPort[STR_MAX]    ;
EXTERN char   acs_LogAddr[STR_MAX]        ;
EXTERN int    acs_LogPort                 ;
EXTERN double acs_LogFreq                 ;
EXTERN struct tm acs_LogInterval          ; // Time between logging 
EXTERN char   str_LogAction[STR_MAX]      ;
EXTERN int    acs_LogAction               ;

EXTERN char   str_ACSNewState[STR_MAX]    ;
EXTERN int    acs_ACSNewState             ;
EXTERN char   acs_ACSReason[STR_MAX]      ;
