/*!
 * Generic ACS header file
 */
#ifdef ACS_MAIN 
#define EXTERN 
#else
#define EXTERN extern
#endif

#define ACS_LOG_DEFAULT 6 

// C standard headers
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <string.h>

// 3rd party headers 
#include <plibsys.h>
#include <sofa.h>

// Build version
#define ACS_VERSION 0.9                   // 0.x = Development 1.x = Release 

// Conversion constants
#define TIME_NOW         NULL
#define NANOSECOND       1000000000L       // No. of nanoseconds in a second 
#define MICROSECOND      1000000.0         // No. of microseconds in a second 
#define TIMETICK         10000000L         // No. of timestamp ticks in a second
#define SECONDS1970      62135596800.0     // Seconds from 1 AD to 1970 AD
#define MAS2DEG          0.001             // Milli-arcsec to degree 
#define MAS2RAD          (DPI/648000000.0) // Milli-arcsec to radians 
#define ACS_STARTPACKAGE 25510027          // ACS packet signature

// Packet command types
#define ACS_HDR_TYPE_GET       1 //
#define ACS_HDR_TYPE_SET       2 //
#define ACS_HDR_TYPE_SUBSCRIBE 3 //
#define ACS_HDR_TYPE_CALLBACK  4 //
#define ACS_HDR_TYPE_ACK       5 //
#define ACS_HDR_TYPE_ERROR     6 //

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
#define ACS_CMD_NODEID         "NODEID"       //  Pseudo command
#define ACS_CMD_TRACKINIT      "TRACKINIT"    // Tracking initialise     Inbound
#define ACS_CMD_TRACKCORR      "TRACKCORR"    // Tracking corrections    Inbound
#define ACS_CMD_TRACKOFFSET    "TRACKOFFSET"  // Tracking offsets        Inbound
#define ACS_CMD_TRACKENABLE    "TRACKENABLE"  // Tracking control        Inbound
#define ACS_CMD_TRACKEND       "TRACKEND"     // Tracking end            Inbound
#define ACS_CMD_TRACKWRITE     "TRACKLOG"     // Write tracking info     Inbound   Engineering
#define ACS_CMD_TRACKREAD      "TRACKREPLAY"  // Read tracking info      Inbound   Engineering
#define ACS_CMD_TRACKRADEC     "TRACKRADEC"   // Tracking to RA/DEC      Inbound
#define ACS_CMD_AGCORR         "AGCORR"       // Autoguider correction   Inbound
#define ACS_CMD_AGDATUM        "AGDATUM"      // Autoguider datum        Inbound
#define ACS_CMD_METINFO        "METINFO"      // Meteorology information Inbound
#define ACS_CMD_INSTINFO       "INSTINFO"     // Instrument information  Inbound
#define ACS_CMD_AXISINFO       "AXISINFO"     // Axis information        Inbound
#define ACS_CMD_AXISDEMAND     "AXISDEMAND"   // Axis demand             Outbound
#define ACS_CMD_ACSGETPARAM    "ACSGETPARAM"  // Get parameter           Inbound
#define ACS_CMD_ACSSETPRARM    "ACSSETPARAM"  // Set parameter           Inbound
#define ACS_CMD_ACSLOG         "ACSLOG"       // Set parameter           Inbound
#define ACS_CMD_ACSREPLAY      "ACSREPLAY"    // Set parameter           Inbound
#define ACS_CMD_ACSRECONFIG    "ACSRECONFIG"  // Re-read configuration   Inbound 
#define ACS_CMD_ACSCHANGE      "ACSCHANGE"    // State change            Outbound
#define ACS_CMD_ACSREPLY       "ACSREPLY"     // Reply                   Outbound

// List of all recognised tags (organised by function 
#define ACS_TAG_COMMANDNAME    "COMMANDNAME"  // Compulsory. Must be first tag
#define ACS_TAG_DEMANDAZM      "DEMANDAZM"
#define ACS_TAG_DEMANDZD       "DEMANDZD"
#define ACS_TAG_DEMANDROT      "DEMANDROTANGLE"
#define ACS_TAG_DEMANDADDRPORT "DEMANDIP"
#define ACS_TAG_TRACKEQUINOX   "TRACKEQUINOX"
#define ACS_TAG_TRACKEPOCH     "TRACKEPOCH"
#define ACS_TAG_TRACKRA        "TRACKRA"
#define ACS_TAG_TRACKDEC       "TRACKDEC"
#define ACS_TAG_TRACKPMRA      "TRACKPMRA"
#define ACS_TAG_TRACKPMDEC     "TRACKPMDEC"
#define ACS_TAG_TRACKPARALLAX  "TRACKPARALLAX"
#define ACS_TAG_TRACKRADVEL    "TRACKRADVEL"
#define ACS_TAG_TRACKROTFRAME  "TRACKROTFRAME"
#define ACS_TAG_TRACKROTANGLE  "TRACKROTANGLE"
#define ACS_TAG_TRACKFREQ      "TRACKFREQ"
#define ACS_TAG_TRACKSTART     "TRACKSTART"
#define ACS_TAG_TRACKDURATION  "TRACKDURATION"
#define ACS_TAG_ENABLEAG       "ENABLEAG"
#define ACS_TAG_ENABLEMET      "ENABLEMET"
#define ACS_TAG_ENABLECORR     "ENABLECORR"
#define ACS_TAG_ENABLEOFFSET   "ENABLEOFFSET"
#define ACS_TAG_ENABLEPMRA     "ENABLEPMRA"
#define ACS_TAG_ENABLEPMDEC    "ENABLEPMDEC"
#define ACS_TAG_ENABLEZD       "ENABLEZD"
#define ACS_TAG_ENABLEAZM      "ENABLEAZM"
#define ACS_TAG_ENABLEROT      "ENABLEROT"
#define ACS_TAG_CORRTYPE       "CORRTYPE"
#define ACS_TAG_CORRFRAME      "CORRFRAME"
#define ACS_TAG_CORRX          "CORRX"
#define ACS_TAG_CORRY          "CORRY"
#define ACS_TAG_CORRROTANGLE   "CORRROTANGLE"
#define ACS_TAG_OFFSETTYPE     "OFFSETTYPE"
#define ACS_TAG_OFFSETFRAME    "OFFSETFRAME"
#define ACS_TAG_OFFSETX        "OFFSETX"
#define ACS_TAG_OFFSETY        "OFFSETY"
#define ACS_TAG_OFFSETROTANGLE "OFFSETROTANGLE"
#define ACS_TAG_AGTYPE         "AGTYPE"
#define ACS_TAG_AGFRAME        "AGFRAME"
#define ACS_TAG_AGX            "AGX"
#define ACS_TAG_AGY            "AGY"
#define ACS_TAG_AGROTANGLE     "AGROTANGLE"
#define ACS_TAG_AGROTATES      "AGROTATES"
#define ACS_TAG_AGWAVELENGTH   "AGWAVELENGTH"
#define ACS_TAG_METTEMP        "METTEMP"
#define ACS_TAG_METPRESS       "METPRESS"
#define ACS_TAG_METRH          "METRH"
#define ACS_TAG_INSTNAME       "INSTNAME"
#define ACS_TAG_INSTROTANGEL   "INSTROTANGLE"
#define ACS_TAG_INSTROTATES    "INSTROTATES"
#define ACS_TAG_INSTWAVELENGTH "INSTWAVELENGTH"
#define ACS_TAG_AXISSTATE      "AXISSTATE"
#define ACS_TAG_AXISZD         "AXISZD"
#define ACS_TAG_AXISAZM        "AXISAZM"
#define ACS_TAG_AXISROTANGLE   "AXISROTANGLE"
#define ACS_TAG_LOGFILE        "LOGFILE"
#define ACS_TAG_LOGADDRPORT    "LOGIP"
#define ACS_TAG_LOGFREQ        "LOGFREQ"
#define ACS_TAG_LOGACTION      "LOGACTION"
#define ACS_TAG_ACSNEWSTATE    "ACSNEWSTATE"
#define ACS_TAG_ACSREASON      "ACSREASON"
#define ACS_TAG_ParamName      "PARAMNAME"
#define ACS_TAG_NODEID         "NODEID"
#define ACS_TAG_NODEIP         "IPADDRESSPORT"
#define ACS_TAG_COMMANDSTR     "COMMANDSTR"
#define ACS_TAG_DATAFIELDSTR   "DATAFIELDSTR"
#define ACS_TAG_TIMESTAMP      "TIMESTAMP"


// Valid parameter names and associated values 
// Parameters are parsed and converted to numerical value for use in code
#define ACS_EMPTY_TXT           ""
#define ACS_ENUM_NOT_FOUND      0

#define ACS_FRAME_CELEST_TXT    "CELESTIAL"
#define ACS_FRAME_MOUNT_TXT     "MOUNT"
#define ACS_FRAME_INST_TXT      "INSTRUMENT"
#define ACS_FRAME_CELEST        1 
#define ACS_FRAME_MOUNT         2 
#define ACS_FRAME_INST          3 
#define ACS_ROTFRAME_CELEST_TXT "CELESTIAL"
#define ACS_ROTFRAME_MOUNT_TXT  "MOUNT"
#define ACS_ROTFRAME_PARAL_TXT  "PARALLACTIC"
#define ACS_ROTFRAME_CELEST     1 
#define ACS_ROTFRAME_MOUNT      2 
#define ACS_ROTFRAME_PARAL      3 
#define ACS_AG_STAR_TXT         "STAR"
#define ACS_AG_WCS_TXT          "WCS"
#define ACS_AG_STAR             1 
#define ACS_AG_WCS              2 
#define ACS_STATE_IDLE_TXT      "IDLE"
#define ACS_STATE_SLEW_TXT      "SLEW"
#define ACS_STATE_TRACK_TXT     "TRACK"
#define ACS_STATE_WARN_TXT      "WARN"
#define ACS_STATE_ERROR_TXT     "ERROR"
#define ACS_STATE_IDLE          0 
#define ACS_STATE_SLEW          1
#define ACS_STATE_TRACK         2
#define ACS_STATE_WARN          3
#define ACS_STATE_ERROR         4
#define ACS_TYPE_FIXED_TXT      "FIXED"
#define ACS_TYPE_CUMUL_TXT      "CUMULATIVE"
#define ACS_TYPE_NONE           0  
#define ACS_TYPE_FIXED          1  
#define ACS_TYPE_CUMUL          2 

//#define ACS_OFFSETTYPE_FIXED_TXT   "FIXED"
//#define ACS_OFFSETTYPE_CUMUL_TXT   "CUMULATIVE"
//#define ACS_OFFSETTYPE_FIXED       1  
//#define ACS_OFFSETTYPE_CUMUL       2 

#define ACS_LOGACTION_REPLAY_TXT "REPLAY"
#define ACS_LOGACTION_GOTO_TXT   "GOTO"
#define ACS_LOGACTION_REPLAY     1 
#define ACS_LOGACTION_GOTO       2  

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
//#define ACS_TSTAMP_FMT  "%FT%T"                // Timestamp format ISO 
#define ACS_LOG_ERR     LOG_WRN_TXT"_"ACS_FAC_LOG_TXT": Failed to write to log file"
#define ACS_SUB_ERR     LOG_ERR_TXT"_"ACS_FAC_ACS_TXT": Failed to spawn sub-process"

// Severity prefix
#define LOG_INF      0                 // Information
#define LOG_WRN      1                 // Warning. Recoverable
#define LOG_ERR      2                 // Fatal error
#define LOG_LIB      3                 // Fatal plibsys or sofa error 
#define LOG_SYS      4                 // Fatal system error 
#define LOG_DBG      5                 // Debug 

// Severity prefix
#define LOG_INF_TXT  "INF"             // Information
#define LOG_WRN_TXT  "WRN"             // Warning. Recoverable or ignorable
#define LOG_ERR_TXT  "ERR"             // Fatal error
#define LOG_LIB_TXT  "LIB"             // Fatal plibsys error 
#define LOG_SYS_TXT  "SYS"             // Fatal system error 
#define LOG_DBG_TXT  "DBG"             // Debug 

// Facility number 
#define ACS_FAC_ACS      0 
#define ACS_FAC_AST      1 
#define ACS_FAC_CHK      2 
#define ACS_FAC_CMD      3 
#define ACS_FAC_INI      4 
#define ACS_FAC_JSN      5 
#define ACS_FAC_LOG      6 
#define ACS_FAC_NET      7 
#define ACS_FAC_UTL      8 

// Facility names
#define ACS_FAC_ACS_TXT  "ACS"
#define ACS_FAC_AST_TXT  "AST"
#define ACS_FAC_CHK_TXT  "CHK"
#define ACS_FAC_CMD_TXT  "CMD"
#define ACS_FAC_INI_TXT  "INI"
#define ACS_FAC_JSN_TXT  "JSN"
#define ACS_FAC_LOG_TXT  "LOG"
#define ACS_FAC_NET_TXT  "NET"
#define ACS_FAC_UTL_TXT  "UTL"

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
    int32_t start;     // Unique marker = 25510027 
    int64_t ID;        // Unique message ID generated by sender
    int32_t count;     // Total number of blocks expected making up message 
    int32_t number;    // The sequence number of this block (starting at 1)
    int32_t jsn_len;   // Length of JSON data
    char    ack;       // ACK expected 0=No 1=Yes
    int32_t type;      // 1=Get 2=Set 3=Subscribe 4=Callback 5=ACK 6=Error
    long    timestamp; //

}  __attribute__((packed)) acs_hdr_t;
#define ACS_HDR_SIZEOF sizeof( acs_hdr_t )

#define ACS_JSN_SIZEOF   ( ACS_NET_PACKET_MAX - ACS_HDR_SIZEOF )
typedef struct acs_pkt_s {
    acs_hdr_t     hdr; 
    unsigned char jsn[ ACS_JSN_SIZEOF ];
} __attribute__((packed)) acs_pkt_t;
#define ACS_PKT_SIZEOF sizeof( acs_pkt_t )

// Used when parsing or writing JSON packets
#define JSN_TYP_END   0
#define JSN_TYP_INT   1
#define JSN_TYP_STR   2
#define JSN_TYP_DBL   3
#define JSN_TYP_LONG  4 
#define JSN_FMT_INT   "\"%s\": %i,"
#define JSN_FMT_DBL   "\"%s\": %f,"
#define JSN_FMT_STR   "\"%s\": \"%s\","

// Define the field width and precision for demand packets. 
// Values below the hash column ## here define the full width and must be the same
#define JSN_FMT_MKDMD "\"%s\": %15.11f ,"
#define JSN_FMT_WRDMD         "%15.11f"

// Data types for tags found in JSON packet 
#define ACS_TYP_UNK          0 // Checks for uninitialised variable 
#define ACS_TYP_INT          1 // integer stored as int
#define ACS_TYP_DBL          2 // double stored as double
#define ACS_TYP_STR          3 // string stored as char[] 
#define ACS_TYP_BOOL         4 // true/false converted stored as bool
#define ACS_TYP_ENUM         5 // A string converted to pseudo enum    
#define ACS_TYP_TIME         6 // A string converted to a time
#define ACS_TYP_IP           7 // A string converted to IP address:port 
#define ACS_TYP_LONG         8 // integer stored as long

// Array data are not handled (yet)  
//#define ACS_TYP_ARRAY_INT     
//#define ACS_TYP_ARRAY_STR     
//#define ACS_TYP_ARRAY_DBL     
//#define ACS_TYP_ARRAY_BOOL    


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


// Global function prototypes
int  acs_close( void );

int   utl_val2enum( unsigned char *val, unsigned char *tbl[] );
int   utl_filexist( char *file );
int   utl_unspace ( char *str );
int   utl_unslash ( char *str );
int   utl_upcase  ( char *str );
int   utl_subchar ( char *str, char old, char new );
int   utl_unchar  ( char *str, char *chars );
char *utl_ts2str  ( struct timespec *ts, char *str );
char *utl_ts2adstr( struct timespec *ts, char *str );
int   utl_ts_cmp( struct timespec *t1, struct timespec *t2 );
long  utl_ts2ad ( struct timespec *ts );
struct timespec utl_ad1tots( long ad1 );
struct timespec utl_dbl2ts( double dbl );
struct timespec utl_ts_add( struct timespec *t1, struct timespec *t2 );
struct timespec utl_ts_sub( struct timespec *t1, struct timespec *t2 );

int  ini_mkmem( void );
int  ini_read ( void );
int  ini_data ( void );
int  acs_ast_calc( struct timespec *ts );

int  log_msg( int ret, int pfx, int fac, char *fmt, ... );
void log_dmp_pkt( acs_pkt_t *pkt ); 

void net_free( acs_net_t *net );
int  net_cmd_ini( acs_net_t *net );
int  net_dmd_ini( acs_net_t *net );
acs_pkt_t *net_cmd_rcv( acs_net_t *net);
long net_dmd_rcv( acs_net_t *net, acs_pkt_t *pkt, long pkt_len );
long net_cmd_snd( acs_net_t *net, acs_pkt_t *pkt, long pkt_len );
long net_dmd_snd( acs_net_t *net, acs_pkt_t *pkt, long pkt_len );
long net_ack_snd( acs_net_t *net, acs_pkt_t *pkt, char *jsn );
char*net_hdr_parse( acs_pkt_t *pkt );
int  net_chk_addr( char *addrport, char *addr, int *port );

int  chk_bool( unsigned char *str, bool   *var, acs_tag_t *tag );
int  chk_int ( unsigned char *str, int    *var, acs_tag_t *tag );
int  chk_long( unsigned char *str, long   *var, acs_tag_t *tag );
int  chk_dbl ( unsigned char *str, double *var, acs_tag_t *tag );
int  chk_str ( unsigned char *str, char   *var, acs_tag_t *tag );
int  chk_enum( unsigned char *str, int    *var, acs_tag_t *tag, acs_enum_t *tbl );
int  chk_ip  ( unsigned char *str, char   *var, acs_tag_t *tag  );
int  chk_time( unsigned char *str, acs_time_t *tim, acs_tag_t *tag );

long jsn_mkpkt( acs_pkt_t  *pkt, unsigned char ack, int type, jsn_tag_t *tags );
long jsn_mkdmd( acs_pkt_t **pkt );
long jsn_wrdmd( acs_pkt_t  *pkt );
bool jsn_parse( unsigned char **pkt, char *expected );
int  acs_tags_cmp( const void *ptr1, const void *ptr2 );


// Function call for each command
int  cmd_Null       (void);
int  cmd_TrackInit  (void);
int  cmd_TrackCorr  (void);
int  cmd_TrackOffset(void);
int  cmd_TrackEnable(void);
int  cmd_TrackEnd   (void);
int  cmd_TrackWrite (void);
int  cmd_TrackRead  (void);
int  cmd_TrackRADec (void);
int  cmd_AGCorr     (void);
int  cmd_AGDatum    (void);
int  cmd_MetInfo    (void);
int  cmd_InstInfo   (void);
int  cmd_AxisInfo   (void);
int  cmd_AxisDemand (void);
int  cmd_ACSGetParam(void);
int  cmd_ACSSetParam(void);
int  cmd_ACSReconfig(void);
int  cmd_ACSChange  (void);
int  cmd_ACSReply   (void);
 
acs_func_t cmd_func( unsigned char *cmd );

#include "acs_dat.h"
#include "acs_tbl.h"
