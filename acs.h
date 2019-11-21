/*!
 * Generic ACS header file
 */
#ifdef ACS_MAIN 
#define EXTERN 
#else
#define EXTERN extern
#endif

// Debug levels
#define DBG0 0  // Off
#define DBG1 1  // Light: command and state changes 
#define DBG2 2  // Medium: file and shared memory update access 
#define DBG3 3  // Verbose:  
#define DBG_DEFAULT DBG0

// C standard headers
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

// plibsys header 
#include <plibsys.h>
// sofa header 
#include <sofa.h>

#define NANOSECOND 1000000000L                // Number of nanoseconds in a second 
#define MAS2DEG          0.001                // Milli-arcsec to degree 
#define MAS2RAD          (DPI/648000000.0)     // Milli-arcsec to radians 
#define ACS_PKT_START 25510027  // ACS packet signature

// Packet command types
#define ACS_PKT_TYPE_GET       1 //
#define ACS_PKT_TYPE_SET       2 //
#define ACS_PKT_TYPE_SUBSCRIBE 3 //
#define ACS_PKT_TYPE_CALLBACK  4 //
#define ACS_PKT_TYPE_ACK       5 //
#define ACS_PKT_TYPE_ERROR     6 //

#define STR_MAX   64  // Max length of a tag in a message
#define DIR_MAX  256  // Max length of a directory spec. 

// Additional function return values
#define ACS_SUCCESS 1 // Function success  
#define ACS_FAILURE 0 // Function failed
#define ACS_ERROR  -1 // Function error

#define ACS_TXT_MEM      "ACS_MEM"          // Memory block name
#define ACS_LEN_INSTNAME  255               // Maximum length of an instrument name
#define ACS_INI_FILE     "acs.ini"          // Default configuration file

// List of ACS Command tags (separated by functional group)     
#define ACS_CMD_TRACKINIT      "TrackInit"    // Tracking initialise     Inbound
#define ACS_CMD_TRACKCORR      "TrackCorr"    // Tracking corrections    Inbound
#define ACS_CMD_TRACKOFFSET    "TrackOffset"  // Tracking offsets        Inbound
#define ACS_CMD_TRACKENABLE    "TrackEnable"  // Tracking control        Inbound
#define ACS_CMD_TRACKEND       "TrackEnd"     // Tracking end            Inbound
#define ACS_CMD_TRACKWRITE     "TrackLog"     // Write tracking info     Inbound   Engineering
#define ACS_CMD_TRACKREAD      "TrackReplay"  // Read tracking info      Inbound   Engineering
#define ACS_CMD_TRACKRADEC     "TrackRADec"   // Tracking to RA/DEC      Inbound
                               
#define ACS_CMD_AGCORR         "AGCorr"       // Autoguider correction   Inbound

#define ACS_CMD_AGDATUM        "AGDatum"      // Autoguider datum        Inbound
                               
#define ACS_CMD_METINFO        "MetInfo"      // Meteorology information Inbound
                               
#define ACS_CMD_INSTINFO       "InstInfo"     // Instrument information  Inbound
                               
#define ACS_CMD_AXISINFO       "AxisInfo"     // Axis information        Inbound
#define ACS_CMD_AXISDEMAND     "AxisDemand"   // Axis demand             Outbound
                               
#define ACS_CMD_ACSGETPARAM    "ACSGetParam"  // Get parameter           Inbound
#define ACS_CMD_ACSSETPRARM    "ACSSetParam"  // Set parameter           Inbound
#define ACS_CMD_ACSLOG         "ACSLog"       // Set parameter           Inbound
#define ACS_CMD_ACSREPLAY      "ACSReplay"    // Set parameter           Inbound
#define ACS_CMD_ACSRECONFIG    "ACSReconfig"  // Re-read configuration   Inbound 
#define ACS_CMD_ACSCHANGE      "ACSChange"    // State change            Outbound
#define ACS_CMD_ACSREPLY       "ACSReply"     // Reply                   Outbound

// List of all recognised tags (organised by function 
#define ACS_TAG_COMMANDNAME    "CommandName"  // Compulsory. Must be first tag

#define ACS_TAG_DEMANDAZM      "DemandAzm"
#define ACS_TAG_DEMANDZD       "DemandZD"
#define ACS_TAG_DEMANDROT      "DemandRotAngle"
#define ACS_TAG_DEMANDADDRPORT "DemandIP"

#define ACS_TAG_TRACKEQUINOX   "TrackEquinox"
#define ACS_TAG_TRACKEPOCH     "TrackEpoch"
#define ACS_TAG_TRACKRA        "TrackRA"
#define ACS_TAG_TRACKDEC       "TrackDec"
#define ACS_TAG_TRACKPMRA      "TrackPMRA"
#define ACS_TAG_TRACKPMDEC     "TrackPMDec"
#define ACS_TAG_TRACKPARALLAX  "TrackParallax"
#define ACS_TAG_TRACKRADVEL    "TrackRadVel"
#define ACS_TAG_TRACKROTFRAME  "TrackRotFrame"
#define ACS_TAG_TRACKROTANGLE  "TrackRotAngle"
#define ACS_TAG_TRACKFREQ      "TrackFreq"
#define ACS_TAG_TRACKSTART     "TrackStart"
#define ACS_TAG_TRACKDURATION  "TrackDuration"

#define ACS_TAG_ENABLEAG       "EnableAG"
#define ACS_TAG_ENABLEMET      "EnableMet"
#define ACS_TAG_ENABLECORR     "EnableCorr"
#define ACS_TAG_ENABLEOFFSET   "EnableOffset"
#define ACS_TAG_ENABLEPMRA     "EnablePMRA"
#define ACS_TAG_ENABLEPMDEC    "EnablePMDec"
#define ACS_TAG_ENABLEZD       "EnableZD"
#define ACS_TAG_ENABLEAZM      "EnableAzm"
#define ACS_TAG_ENABLEROT      "EnableRot"

#define ACS_TAG_CORRTYPE       "CorrType"
#define ACS_TAG_CORRFRAME      "CorrFrame"
#define ACS_TAG_CORRX          "CorrX"
#define ACS_TAG_CORRY          "CorrY"
#define ACS_TAG_CORRROTANGLE   "CorrRotAngle"

#define ACS_TAG_OFFSETTYPE     "OffsetType"
#define ACS_TAG_OFFSETFRAME    "OffsetFrame"
#define ACS_TAG_OFFSETX        "OffsetX"
#define ACS_TAG_OFFSETY        "OffsetY"
#define ACS_TAG_OFFSETROTANGLE "OffsetRotAngle"

#define ACS_TAG_AGTYPE         "AGType"
#define ACS_TAG_AGFRAME        "AGFrame"
#define ACS_TAG_AGX            "AGX"
#define ACS_TAG_AGY            "AGY"
#define ACS_TAG_AGROTANGLE     "AGRotAngle"
#define ACS_TAG_AGROTATES      "AGRotates"
#define ACS_TAG_AGWAVELENGTH   "AGWavelength"

#define ACS_TAG_METTEMP        "MetTemp"
#define ACS_TAG_METPRESS       "MetPress"
#define ACS_TAG_METRH          "MetRH"

#define ACS_TAG_INSTNAME       "InstName"
#define ACS_TAG_INSTROTANGEL   "InstRotAngle"
#define ACS_TAG_INSTROTATES    "InstRotates"
#define ACS_TAG_INSTWAVELENGTH "InstWavelength"

#define ACS_TAG_AXISSTATE      "AxisState"
#define ACS_TAG_AXISZD         "AxisZD"
#define ACS_TAG_AXISAZM        "AxisAzm"
#define ACS_TAG_AXISROTANGLE   "AxisRotAngle"

#define ACS_TAG_LOGFILE        "LogFile"
#define ACS_TAG_LOGADDRPORT    "LogIP"
#define ACS_TAG_LOGFREQ        "LogFreq"
#define ACS_TAG_LOGACTION      "LogAction"

#define ACS_TAG_ACSNEWSTATE    "ACSNewState"
#define ACS_TAG_ACSREASON      "ACSReason"

#define ACS_TAG_ParamName      "ParamName"


// Valid parameter names and associated values 
// Parameters are parsed and converted to numerical value for use in code
#define ACS_EMPTY_TXT            ""
#define ACS_ENUM_NOT_FOUND       0

#define ACS_FRAME_CELEST_TXT     "CELESTIAL"
#define ACS_FRAME_MOUNT_TXT      "MOUNT"
#define ACS_FRAME_INST_TXT       "INSTRUMENT"
#define ACS_FRAME_CELEST         1 
#define ACS_FRAME_MOUNT          2 
#define ACS_FRAME_INST           3 

#define ACS_ROTFRAME_CELEST_TXT  "CELESTIAL"
#define ACS_ROTFRAME_MOUNT_TXT   "MOUNT"
#define ACS_ROTFRAME_PARAL_TXT   "PARALLACTIC"
#define ACS_ROTFRAME_CELEST      1 
#define ACS_ROTFRAME_MOUNT       2 
#define ACS_ROTFRAME_PARAL       3 

#define ACS_AG_STAR_TXT          "STAR"
#define ACS_AG_WCS_TXT           "WCS"
#define ACS_AG_STAR              1 
#define ACS_AG_WCS               2 

#define ACS_STATE_IDLE_TXT       "IDLE"
#define ACS_STATE_SLEW_TXT       "SLEW"
#define ACS_STATE_TRACK_TXT      "TRACK"
#define ACS_STATE_WARN_TXT       "WARN"
#define ACS_STATE_ERROR_TXT      "ERROR"
#define ACS_STATE_IDLE           0 
#define ACS_STATE_SLEW           1
#define ACS_STATE_TRACK          2
#define ACS_STATE_WARN           3
#define ACS_STATE_ERROR          4

#define ACS_TYPE_FIXED_TXT   "FIXED"
#define ACS_TYPE_CUMUL_TXT   "CUMULATIVE"
#define ACS_TYPE_NONE        0  
#define ACS_TYPE_FIXED       1  
#define ACS_TYPE_CUMUL       2 

//#define ACS_OFFSETTYPE_FIXED_TXT   "FIXED"
//#define ACS_OFFSETTYPE_CUMUL_TXT   "CUMULATIVE"
//#define ACS_OFFSETTYPE_FIXED       1  
//#define ACS_OFFSETTYPE_CUMUL       2 

#define ACS_LOGACTION_REPLAY_TXT    "REPLAY"
#define ACS_LOGACTION_GOTO_TXT      "GOTO"
#define ACS_LOGACTION_REPLAY        1 
#define ACS_LOGACTION_GOTO          2  

#define ACS_EQUINOX_B1950_0_TXT  "B1950.0"  
#define ACS_EQUINOX_J2000_0_TXT  "J2000.0" 
#define ACS_EQUINOX_ICRS_TXT     "ICRS" 
#define ACS_EQUINOX_TIME_TXT     "TIME" 
#define ACS_EQUINOX_B1950_0      1
#define ACS_EQUINOX_J2000_0      2
#define ACS_EQUINOX_ICRS         3
#define ACS_EQUINOX_TIME         4

#define ACS_DIR_INBOUND          1
#define ACS_DIR_OUTBOUND         2

#define ACS_SMOOTHING_NONE_TXT   "NONE" 
#define ACS_SMOOTHING_MEAN_TXT   "MEAN" 
#define ACS_SMOOTHING_NONE       1 
#define ACS_SMOOTHING_MEAN       2 

#define ACS_PROTOCOL_TCP_TXT     "TCP"
#define ACS_PROTOCOL_UDP_TXT     "UDP"


// Logging 
#define ACS_LOG_FILE    "/tmp/acs_errlog.log"  // Default log file
#define ACS_TSTAMP_FMT  "%FT%T"                // Timestamp format ISO 
#define ACS_LOG_ERR     ACS_PFX_WRN_TXT"_"ACS_FAC_LOG_TXT": Failed to write to log file"
#define ACS_SUB_ERR     ACS_PFX_ERR_TXT"_"ACS_FAC_ACS_TXT": Failed to spawn sub-process"

// Severity prefix
#define ACS_PFX_INF      0                 // Information
#define ACS_PFX_WRN      1                 // Warning. Recoverable or ignorable
#define ACS_PFX_ERR      2                 // Fatal error
#define ACS_PFX_LIB      3                 // Fatal plibsys error 
#define ACS_PFX_SYS      4                 // Fatal system error 
#define ACS_PFX_DBG      5 

// Severity prefix
#define ACS_PFX_INF_TXT  "INF"             // Information
#define ACS_PFX_WRN_TXT  "WRN"             // Warning. Recoverable or ignorable
#define ACS_PFX_ERR_TXT  "ERR"             // Fatal error
#define ACS_PFX_LIB_TXT  "LIB"             // Fatal plibsys error 
#define ACS_PFX_SYS_TXT  "SYS"             // Fatal system error 
#define ACS_PFX_DBG_TXT  "DBG"             // Debug 

// Facility number 
#define ACS_FAC_ACS      0 
#define ACS_FAC_CFG      1 
#define ACS_FAC_NET      2 
#define ACS_FAC_LOG      3 
#define ACS_FAC_MEM      4 
#define ACS_FAC_JSN      5 
#define ACS_FAC_CHK      6 

// Facility names
#define ACS_FAC_ACS_TXT  "ACS"
#define ACS_FAC_CFG_TXT  "CFG"
#define ACS_FAC_NET_TXT  "NET"
#define ACS_FAC_LOG_TXT  "LOG"
#define ACS_FAC_MEM_TXT  "MEM"
#define ACS_FAC_JSN_TXT  "JSN"
#define ACS_FAC_CHK_TXT  "CHK"

#define ACS_NET_PACKET_MAX 65535

// Time structure
typedef struct acs_time_s {
    char      str[STR_MAX];
    double    utc[2];
    struct tm tim;

} acs_time_t; 


// Shared memory structure 
typedef struct acs_mem_s {
// IPC variables
    bool   Ready;         // Shared memory data is valid and ready to be used
    double ReqRadRA;      // TrackRa  + Corr. + Offset + AG offset in radians
    double ReqRadDec;     // TrackDec + Corr. + Offset + AG offset in radians
    double DeltaTime;     // [s]  
    double DeltaRot;      // [deg]
    double DeltaAzm;      // [deg]
    double DeltaZD;       // [deg]

// Astrometric engine data
    double Lat;    // [deg] Latitude
    double Lon;    // [deg] Longitude
    double Alt;    // [m]   Altitude above sea level
    double PolarX; // 
    double PolarY; //
    double Press;  // [mB] Pressure 0.0 = suppress correction
    double Temp;   // [C]  Temperature
    double RH;     //      Relative humidity 0.0-1.0
    double DUT1;   // [s]  Delta UT offset 

// Tracking
    bool   EnablePMRA;
    bool   EnablePMDec;
    bool   EnableZD;
    bool   EnableAzm;
    bool   EnableRot;

    int    TrackEquinox;
    acs_time_t TrackEpoch;
    double TrackRA;
    double TrackDec;
    double TrackPMRA;
    double TrackPMDec;
    double TrackParallax;
    double TrackRadVel;
    int    TrackRotFrame;
    double TrackRotAngle;
    double TrackFreq;
    struct timespec DemandInterval;
    double TrackDest;
    acs_time_t TrackStart;
    double TrackDuration;

// Correction
    bool   EnableCorr;
    int    CorrType;
    int    CorrFrame;
    double CorrX;
    double CorrY;
    double CorrRot;

// Offset
    bool   EnableOffset;
    int    OffsetType;
    int    OffsetFrame;
    double OffsetX;
    double OffsetY;   
    double OffsetRot;

//Autoguider
    bool   EnableAG;
    int    AGFrame;
    double AGX;
    double AGY;
    int    AGType;
    double AGRotAngle;
    bool   AGRotates;
    double AGWavelength;

// Weather 
    bool   EnableMet;
    double MetTemp;
    double MetPress;
    double MetRH; 

// Instrument
    char   InstName[ACS_LEN_INSTNAME+1]; 
    double InstRotAngle;
    bool   InstRotates;
    double InstWavelength;   

//  Demands to be sent
    double DmdAzm;
    double DmdZD;
    double DmdRot;
    double DmdRA;
    double DmdDec;
    double DmdEO;

} acs_mem_t;

// IP address
typedef struct acs_ip_s {
    char Addr[32];
    int  Port;
} acs_ip_t;

// Network socket structure
typedef struct acs_net_s {
    bool  ok                          ; // Connection is working
    bool  isserver                    ; // Operating mode true=server, false=client
    enum  PSocketProtocol_ sktProtocol; // plibsys protocol
    char                  *Protocol;    // protocol as defined in global acs_IPProtcol
    char                  *Addr;        // IP address as read from .ini
    int                    Port;        // IP port as read from .ini
    PSocketAddress        *sktAddr ;    // Socket address 
    PSocket               *sktConn ;    // Established connection
    PSocket               *sktServ ;    // Server socket for inbound connections
    PError                *sktError;    // Socket error
} acs_net_t;

typedef struct acs_hdr_s {
    int32_t start;  // Unique marker = 25510027 
    int64_t ID;     // Unique message ID generated by sender
    int32_t total;  // Total number of blocks expected making up message 
    int32_t seqn;   // The sequence number of this block (starting at 1)
    int32_t len;    // Block length
    char    ack;    //  
    int32_t type;
} acs_hdr_t;


typedef struct acs_pkt_s {
    acs_hdr_t     hdr; 
    unsigned char jsn[ACS_NET_PACKET_MAX - sizeof(acs_hdr_t)];
} acs_pkt_t;


// Used when parsing or writing JSON packets
#define JSN_TYP_END   0
#define JSN_TYP_INT   1
#define JSN_TYP_STR   2
#define JSN_TYP_DBL   3
#define JSN_FMT_INT   "\"%s\": %i,"
#define JSN_FMT_DBL   "\"%s\": %f,"
#define JSN_FMT_STR   "\"%s\": \"%s\","

// Define the field width and precision for demand packets. 
// Values below the hash column ## here define the full width and must be the same
#define JSN_FMT_MKDMD "\"%s\": %15.11f ,"
#define JSN_FMT_WRDMD         "%15.11f"

// ACS data types for tags found in JSON packet 
#define ACS_TYP_UNK          0 // Checks for uninitialised variable 
#define ACS_TYP_INT          1 // integer stared as int
#define ACS_TYP_DBL          2 // double stored as douoble
#define ACS_TYP_STR          3 // string stored as string
#define ACS_TYP_BOOL         4 // true/false converted stored as bool
#define ACS_TYP_ENUM         5 // A string converted to pseudo enum    
#define ACS_TYP_TIME         6 // A string converted to a time
#define ACS_TYP_IP           7 // A string converted to a time

// Array data are not handled (yet) but are inlcuded for completeness 
#define ACS_TYP_ARRAY_INT    8 
#define ACS_TYP_ARRAY_STR    9 
#define ACS_TYP_ARRAY_DBL    10 
#define ACS_TYP_ARRAY_BOOL   11 


// JSON tag item used to construct message array
typedef struct jsn_tag_s {
    char *name;  // Pointer to tag name string
    int   type;  // Tag type
    char *fmt;   // Format when sprintf-ing output string
    void *val;   // Pointer to tag's value 
} jsn_tag_t;

// ACS Configuration Table 
typedef struct acs_cfg_s {
    char *key;  // Key name
    char *sect; // Section name
    int   type; // Data type
    void *ptr;  // Pointer to variable
//  void *dflt; // Default value
//  double lo;  // Sanity check 
//  double hi;  // Sanity check
} acs_cfg_t;


// ACS Command Table
typedef int (*acs_func_t)(void);
typedef struct acs_cmd_s {
    char *name;      // Command name
    acs_func_t func; // Command function  
    int   dir;       // Command direction (inbound or outbound)
} acs_cmd_t;

// ACS Tag Table
typedef struct acs_tag_s {
    char  *name; // Tag name
    int    type; // Variable type
    void  *var;  // Pointer to variable 
    void  *chk;  // Check function, either numeric range or string  
    double min;  // Numeric minimum check value
    double max;  // Numeric maximum check value
    void  *str;  // String check values   
} acs_tag_t;

// Conversion tables for string to pseudo-enum
typedef struct acs_enum_s {
     unsigned char *str;
     int            val;
} acs_enum_t;


// Macro prototypes
#define acs_debug( lvl, fmt, ...) acs_dbg_lvl >= lvl ? printf( "DBG: "fmt"\n", __VA_ARGS__):0

// Global function prototypes
int acs_close( void );

int              utl_val2enum( unsigned char *val, unsigned char *tbl[] );
acs_func_t       utl_cmd2func( unsigned char *cmd );
struct timespec *utl_dbl2ts( double dbl );
struct timespec  utl_ts_add( struct timespec *t1, struct timespec *t2 );
struct timespec  utl_ts_sub( struct timespec *t1, struct timespec *t2 );
int              utl_ts_cmp( struct timespec *t1, struct timespec *t2 );
int              utl_filexist( char *file );

int acs_ini_mkmem( void );
int acs_ini_mkmem2( void );
int acs_ini_read ( void );
int acs_ini_data ( void );
int acs_ast_calc ( struct timespec *ts );

int acs_log_msg( int ret, int pfx, int fac, char *fmt, ... );

int      acs_net_cmd_ini( acs_net_t *net );
int      acs_net_dmd_ini( acs_net_t *net );
void     acs_net_free( acs_net_t *net );
long int acs_net_cmd_rcv( acs_net_t *net, unsigned char *buf, long int siz );
long int acs_net_cmd_snd( acs_net_t *net, unsigned char *buf, long int len );
long int acs_net_dmd_rcv( acs_net_t *net, unsigned char *buf, long int siz );
long int acs_net_dmd_snd( acs_net_t *net, unsigned char *buf, long int len );
int      acs_net_chk( char *AddrPort, char *Addr, int *Port  );

int acs_chk_int ( unsigned char *str, int    *var, acs_tag_t *tag );
int acs_chk_dbl ( unsigned char *str, double *var, acs_tag_t *tag );
int acs_chk_str ( unsigned char *str, char   *var, acs_tag_t *tag );
int acs_chk_bool( unsigned char *str, bool   *var, acs_tag_t *tag );
int acs_chk_enum( unsigned char *str, int    *var, acs_tag_t *tag, acs_enum_t *tbl );
int acs_chk_ip  ( unsigned char *str, char   *var, acs_tag_t *tag  );
int acs_chk_time( unsigned char *str, acs_time_t *tim, acs_tag_t *tag );

long int acs_jsn_mkpkt( acs_pkt_t  *pkt, unsigned char ack, int type, jsn_tag_t *tags );
long int acs_jsn_mkdmd( acs_pkt_t **pkt );
long int acs_jsn_wrdmd( acs_pkt_t  *pkt );
bool     acs_jsn_parse( unsigned char *pkt );
int      acs_tags_cmp( const void *ptr1, const void *ptr2 );

// Function call for each command
int acs_cmd_TrackInit  (void);
int acs_cmd_TrackCorr  (void);
int acs_cmd_TrackOffset(void);
int acs_cmd_TrackEnable(void);
int acs_cmd_TrackEnd   (void);
int acs_cmd_TrackWrite (void);
int acs_cmd_TrackRead  (void);
int acs_cmd_TrackRADec (void);
int acs_cmd_AGCorr     (void);
int acs_cmd_AGDatum    (void);
int acs_cmd_MetInfo    (void);
int acs_cmd_InstInfo   (void);
int acs_cmd_AxisInfo   (void);
int acs_cmd_AxisDemand (void);
int acs_cmd_ACSGetParam(void);
int acs_cmd_ACSSetParam(void);
int acs_cmd_ACSReconfig(void);
int acs_cmd_ACSChange  (void);
int acs_cmd_ACSReply   (void);

#include "acs_dat.h"
#include "acs_tbl.h"
