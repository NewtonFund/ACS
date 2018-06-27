/* Global data storage
 *
*/
#ifdef ACS_MAIN 
#define EXTERN 
#else
#define EXTERN extern
#endif

EXTERN int acs_dbg_lvl;
EXTERN pid_t acs_cmdpid;
EXTERN pid_t acs_dmdpid;
EXTERN acs_mem_t *acs_mem;

// Configuratin data read from .ini file
EXTERN char   acs_Latitude[32]   ; // String 
EXTERN char   acs_Longitude[32]  ; // String 
EXTERN double acs_Altitude       ; // Decimal

EXTERN char   acs_IPType[99]     ; // String 
EXTERN char   acs_IPAddress[16]  ; // String 
EXTERN int    acs_IPPort         ; // Port

EXTERN char   acs_Dir            ; // String 

EXTERN double acs_LimitDecHi     ; // Decimal
EXTERN double acs_LimitDecLo     ; // Decimal
EXTERN double acs_LimitZDHi      ; // Decimal
EXTERN double acs_LimitZDlo      ; // Decimal
EXTERN double acs_LimitAzmHi     ; // Decimal
EXTERN double acs_LimitAzmLo     ; // Decimal
EXTERN double acs_LimitRotHi     ; // Decimal
EXTERN double acs_LimitRotLo     ; // Decimal

EXTERN double acs_MaxErrZD       ; // Decimal
EXTERN double acs_MaxErrAzm      ; // Decimal
EXTERN double acs_MaxErrRot      ; // Decimal

EXTERN double acs_TimeoutSlew    ; // Decimal
EXTERN double acs_TimeoutAG      ; // Decimal
EXTERN double acs_TimeoutMet     ; // Decimal
EXTERN double acs_TimeoutAxis    ; // Decimal
EXTERN double acs_TimeoutReply   ; // Decimal
EXTERN double acs_TimeoutMessage ; // Decimal

EXTERN double acs_LeapSecs       ; // Decimal
EXTERN double acs_DUTC           ; // Decimal
EXTERN double acs_PolarX         ; // Decimal
EXTERN double acs_PolarY         ; // Decimal

EXTERN char   acs_MessageIPAddress[16]  ; // String 
EXTERN int    acs_MessageIPPort  ; // String 
EXTERN char   acs_DemandIPAddress[16]   ; // String 
EXTERN int    acs_DemandIPPort   ; // String 
EXTERN int    acs_DemandFreq     ; // Integer

EXTERN char    acs_AGIPAddress[16]; // String 
EXTERN int     acs_AGIPPort      ; // Integer 
EXTERN char    acs_AGFrame[99]   ; // String 
EXTERN char    acs_AGType[99]    ; // String 
EXTERN double  acs_AGRotAngle    ; // Decimal
EXTERN double  acs_AGWavelength  ; // Decimal

EXTERN char    acs_AGSmoothing[99]; // String 
EXTERN int     acs_AGSampleSize   ; // Integer

EXTERN char    acs_MetIPAddress[16]; // String 
EXTERN int     acs_MetIPPort     ; // Integer 
EXTERN double  acs_MetPress      ; // Decimal
EXTERN double  acs_MetTemp       ; // Decimal
EXTERN double  acs_MetRH         ; // Decimal
EXTERN double  acs_Wavelength    ; // Decimal
EXTERN char    acs_Smoothing     ; // String 
EXTERN int     acs_SampleSize    ; // Integer

EXTERN char     acs_InstName[99]  ; // String 
EXTERN double   acs_InstRotAngle  ; // Decimal
EXTERN double   acs_Wavelength    ; // Decima

