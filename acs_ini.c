/*!
 * Initialisation and startup functions 
 */

#include "acs.h"

#define SHM_NAME "/ACS_MEM"
#define SHM_OPTS (O_RDWR | O_CREAT)
#define SHM_PROT (PROT_READ | PROT_WRITE)
#define SHM_MODE (00666 )
int   shm_fd;
char *shm_ptr;

/*!
 * Comparison function used by qsort() to sort acs_tag table into ascending alphabetical order 
 */
int acs_tags_cmp( const void *ptr1, const void *ptr2 )
{
    return strcmp( ((acs_tag_t *)ptr1)->name, ((acs_tag_t *)ptr2)->name);
}

/*
 * Comparison function used by qsort() to sort acs_cmd table into ascending alphabetical order 
 */
int acs_cmds_cmp( const void *ptr1, const void *ptr2 )
{
    return strcmp( ((acs_cmd_t *)ptr1)->name, ((acs_cmd_t *)ptr2)->name);
}

/*!
 * Create and zero shared memory block 
*/
int acs_ini_mkmem(void)
{
    PError *error;

    const PShm *shm;

    if (( shm     = p_shm_new( ACS_TXT_MEM, sizeof( acs_mem_t ), P_SHM_ACCESS_READWRITE, &error ))&&
        ( acs_mem = p_shm_get_address( shm )                                                     )  )
    {
        memset( acs_mem, 0, sizeof( acs_mem_t ) );
        return ACS_SUCCESS;
    }
    else
    {
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_ACS, "p_shm_new() failed, acs_mem=%p", acs_mem );
    }
}


int acs_ini_mkmem2(void)
{
    if (((shm_fd  = shm_open( SHM_NAME, SHM_OPTS, SHM_MODE )                 ) > 0)&& // Get shared memory file desc.
        ( ftruncate(shm_fd,   sizeof(acs_mem_t)            )                  == 0)&& // Force size
        ((acs_mem = mmap(NULL,sizeof(acs_mem_t),SHM_PROT,MAP_SHARED,shm_fd,0)) > 0)  )// Map shared memory
    {
        return true;
    }

    return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_ACS, "acs_ini_mkmem2() failed" );
}


/*!
 * Read configuration .ini file
 */
int acs_ini_read( void )
{
    int i;

    PIniFile *file;
    PError   *error;

    file = p_ini_file_new( ACS_INI_FILE );

    if ( !p_ini_file_parse( file, &error ) )
    {
        return acs_log_msg( ACS_FAILURE, ACS_PFX_ERR, ACS_FAC_CFG, "Failed to read .ini file" );
    }
    else
    {
        for( i = acs_cfg_num; i-- ; ) {
            switch (acs_cfg[i].type) {
                case CFG_TYPE_INT:
                    *(int *)acs_cfg[i].ptr = p_ini_file_parameter_int( file, acs_cfg[i].sect, acs_cfg[i].key, 0 );
                    acs_debug( DBG3, "%s=%i", acs_cfg[i].key, *(int *)acs_cfg[i].ptr );
                    break;

                case CFG_TYPE_DOUBLE:
                    *(double *)acs_cfg[i].ptr = p_ini_file_parameter_double( file, acs_cfg[i].sect, acs_cfg[i].key, 0.0 );
                    acs_debug( DBG3, "%s=%f", acs_cfg[i].key, *(double *)acs_cfg[i].ptr );
                    break;

                case CFG_TYPE_BOOLEAN:
                    *(unsigned char *)acs_cfg[i].ptr = p_ini_file_parameter_boolean( file, acs_cfg[i].sect, acs_cfg[i].key, FALSE );
                    acs_debug( DBG3, "%s=%s", acs_cfg[i].key, *(int *)acs_cfg[i].ptr ? "TRUE":"FALSE");
                    break;

                case CFG_TYPE_STRING:
                    strcpy( (char *)acs_cfg[i].ptr, (char *)p_ini_file_parameter_string( file, acs_cfg[i].sect, acs_cfg[i].key, ""));
                    acs_debug( DBG3, "%s=%s", acs_cfg[i].key, (char *)acs_cfg[i].ptr );
                    break;

                default:
                    break;
            }
        }   
    }
    return ACS_SUCCESS;
}


/*
 *  Final evaluation and init. of running data.
 *  Includes sanity checks  
 */
int acs_ini_data(  void )
{
    char   LatSgn;
    int    LatDeg;
    int    LatMin;
    double LatSec;

    char   LonSgn;
    int    LonDeg;
    int    LonMin;
    double LonSec;

    struct timespec before;
    struct timespec after;

//  Sort the global tag table into alphabetical order so it can be searched using bsearch()
    qsort( acs_tags, acs_tags_num, acs_tags_siz, acs_tags_cmp );
    qsort( acs_cmds, acs_cmds_num, acs_cmds_siz, acs_cmds_cmp );

/*  Check latitude and longitude is within limits and convert to radians */
    if (( 3 == sscanf( str_Lat, "%i:%i:%lf", &LatDeg, &LatMin, &LatSec ))&&
        ( 3 == sscanf( str_Lon, "%i:%i:%lf", &LonDeg, &LonMin, &LonSec ))  ) 
    {
/*      Convert +/-DD:MM:SS.sss  latitude and longitude to radians */
        LatSgn = LatDeg > 0 ? '+':'-'; 
        LonSgn = LonDeg > 0 ? '+':'-'; 
        iauAf2a ( LatSgn, LatDeg, LatMin, LatSec, &acs_mem->Lat );
        iauAf2a ( LonSgn, LonDeg, LonMin, LonSec, &acs_mem->Lon );

/*      Check Lat. & Long. limits */    
        if (( acs_mem->Lat <=  DPI/2.0 )&&  
            ( acs_mem->Lat >= -DPI/2.0 )&&  
            ( acs_mem->Lon <=  DPI     )&&  
            ( acs_mem->Lon >= -DPI     )  ) 
        {
            acs_mem->Alt = acs_Alt;
            acs_debug( DBG3, "Lat=%s=%lf [rad] Lon=%s=%lf [rad]", str_Lat, acs_mem->Lat, str_Lon, acs_mem->Lon );
        }
        else
        {
            return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_CFG,
                                "Location out of limits: Latitude=%f Longitude=%f [rad]",
                                acs_mem->Lat, acs_mem->Lon );  
        } 
    }
    else
    {
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_CFG,
                            "Invalid location format: Latitude=%s Longitude=%s [+/-DDD:MM.SS.sss]",
                            str_Lat, str_Lon );  
    }

/*  Identify the IP protocol to be used */
    if ( !strcmp( str_Protocol, "TCP" ))
        acs_sktProtocol = P_SOCKET_PROTOCOL_TCP;
    else if ( !strcmp( str_Protocol, "UDP" ))
        acs_sktProtocol = P_SOCKET_PROTOCOL_UDP;
    else
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_CFG, "Invalid network protocol=%s", str_Protocol );  
    acs_debug( DBG3, "Network protocol=%s", str_Protocol );

//  Unpack and check network port addresses and ports */    
    if ( !acs_net_chk( str_CommandAddrPort,  acs_CommandAddr, &acs_CommandPort ) ) 
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_CFG, "Bad Command IP Address:Port=%s", str_CommandAddrPort );  

    if ( !acs_net_chk( str_DemandAddrPort,   acs_DemandAddr,  &acs_DemandPort  ) ) 
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_CFG, "Bad Demand IP Address:Port=%s", str_DemandAddrPort );  

//  Init. network interface for incoming commands 
    acs_net_cmd.Protocol   = str_Protocol;
    acs_net_cmd.Addr       = acs_CommandAddr;
    acs_net_cmd.Port       = acs_CommandPort;
    acs_net_cmd.sktProtocol= acs_sktProtocol;
    acs_net_cmd.sktServ    = NULL;
    acs_net_cmd.sktConn    = NULL;
    acs_net_cmd.sktAddr    = NULL;
    acs_net_cmd.ok         = false;
    acs_net_cmd.isserver   = true;

//  Init. nework interface for the outgoing demands 
    acs_net_dmd.Protocol   = str_Protocol;
    acs_net_dmd.Addr       = acs_DemandAddr;
    acs_net_dmd.Port       = acs_DemandPort;
    acs_net_dmd.sktProtocol= acs_sktProtocol;
    acs_net_dmd.sktServ    = NULL;
    acs_net_dmd.sktConn    = NULL;
    acs_net_dmd.sktAddr    = NULL;
    acs_net_dmd.ok         = false;
    acs_net_dmd.isserver   = false;

//  Copy config data into shared block
    strcpy( acs_mem->InstName, acs_InstName );

//  Initialise timer variables

//  First evaluate the timer interval ...
    acs_DemandInterval.tv_sec  = acs_mem->DemandInterval.tv_sec  = (long int)(NANOSECOND / acs_DemandFreq) / NANOSECOND;
    acs_DemandInterval.tv_nsec = acs_mem->DemandInterval.tv_nsec = (long int)(NANOSECOND / acs_DemandFreq) % NANOSECOND;

//  ... then estimate the time for an astrometric calculation  
    clock_gettime( CLOCK_REALTIME, &before );
    acs_ast_calc( &before );
    clock_gettime( CLOCK_REALTIME, &after );
    acs_DemandLatency = utl_ts_sub( &after, &before ); 

    acs_log_msg( true, ACS_PFX_INF, ACS_FAC_CFG, "Astrometric Calc. latency=%li.%li sec", 
                 acs_DemandLatency.tv_sec, acs_DemandLatency.tv_nsec );

//  Warn if the time to calculate a demand (latency) is greater than the demand interval
//  and set the latency to {0,0} to run as fast as possible
    if ( utl_ts_cmp( &acs_DemandInterval, &acs_DemandLatency ) )
    {
        acs_log_msg( true, ACS_PFX_WRN, ACS_FAC_CFG, "Latency=%li.%li [sec] greater than demand interval=%li.%li [sec]",
                     acs_DemandLatency.tv_sec, acs_DemandLatency.tv_nsec, acs_DemandInterval.tv_sec, acs_DemandInterval.tv_nsec );  
        acs_DemandLatency.tv_sec  = 0;
        acs_DemandLatency.tv_nsec = 0;
    }

    return ACS_SUCCESS;
}

/*!
 * Initialise global data variables to "safe" default values.
 * These may be overwritten by configuration file defaults
 * No checks or conversions
 */
int acs_ini_global( void )
{
strcpy(str_Lat,             "+18:35:15.1234")       ;
strcpy(str_Lon,             "+98:29:12.1234")       ;
acs_Alt                     =    0.0                ;

acs_sktProtocol             =P_SOCKET_PROTOCOL_TCP  ;
strcpy( str_Protocol,       "TCP")                  ; 
strcpy( str_ACSAddrPort,    "127.0.0.1:14000")      ;
strcpy( acs_Addr,           "127.0.0.1")            ;
acs_Port                    =14000                  ;
                                                    
strcpy( acs_Dir,            "./")                   ;
                                                    
acs_LimitZDHi               =   90.0                ;
acs_LimitZDlo               =    0.0                ;
acs_LimitAzmHi              =  180.0                ;
acs_LimitAzmLo              = -180.0                ;
acs_LimitRotHi              =  180.0                ;
acs_LimitRotLo              = -180.0                ;
                                                    
acs_MaxErrZD                =    1.0                ;
acs_MaxErrAzm               =    1.0                ;
acs_MaxErrRot               =    1.0                ;
                                                    
// -ve timeout = infinite                                      
acs_TimeoutSlew             =   -1.0                ;
acs_TimeoutAG               =   -1.0                ;
acs_TimeoutMet              =   -1.0                ;
acs_TimeoutAxis             =   -1.0                ;
acs_TimeoutReply            =   -1.0                ;
acs_TimeoutAge              =   -1.0                ;
                                                    
acs_LeapSecs                =   37.0                ;
acs_DUTC                    =    0.34               ;
acs_PolarX                  =    0.0                ;
acs_PolarY                  =    0.0                ;
                                                    
strcpy(str_CommandAddrPort, "127.0.0.1:14111")      ;
strcpy(acs_CommandAddr,     "127.0.0.1")            ;
acs_CommandPort             =14111                  ;
                                                    
strcpy(str_DemandAddrPort,  "127.0.0.1:14222")      ;
strcpy(acs_DemandAddr,      "127.0.0.1")            ;
acs_DemandPort              =14222                  ;
acs_DemandFreq                                      ;

strcpy(str_AGAddrPort,      "127.0.0.1:14333")      ;
strcpy(acs_AGAddr,          "127.0.0.1")            ;
acs_AGPort                  =14333                  ;
strcpy(str_AGFrame,          ACS_FRAME_CELEST_TXT)  ;
acs_AGFrame                 =ACS_FRAME_CELEST       ;
strcpy(str_AGType,           ACS_AG_STAR_TXT)       ;
acs_AGType                  =ACS_AG_STAR            ;
acs_AGRotates               =false                  ;
acs_AGX                     =    0.0                ;
acs_AGY                     =    0.0                ;
acs_AGRotAngle              =    0.0                ;
acs_AGWavelength            =  550.0                ;
strcpy(str_AGSmoothing,      ACS_SMOOTHING_NONE_TXT);
acs_AGSmoothing             =ACS_SMOOTHING_NONE     ;
acs_AGSampleSize            =    3                  ;

strcpy(str_MetAddrPort,     "127.0.0.1:14444")      ;
strcpy(acs_MetAddr,         "127.0.0.1"      )      ;
acs_MetPort                 =14444                  ;
acs_MetPress                = 1000.0                ;
acs_MetTemp                 =   20.0                ;
acs_MetRH                   =    0.5                ;
acs_MetWavelength           =  550.0                ;
strcpy(str_MetSmoothing,     ACS_SMOOTHING_NONE_TXT);
acs_MetSmoothing            =ACS_SMOOTHING_NONE     ;

strcpy(acs_InstName,        "INSTRUMENT" )          ;
acs_InstRotAngle            =    0.0                ;
acs_InstWavelength          =  550.0                ;
acs_InstRotates             =false                  ;

acs_TrackEquinox            =ACS_EQUINOX_J2000_0    ;
//acs_time_t acs_TrackEpoch                         
acs_TrackRA                 =    0.0                ;
acs_TrackDec                =    0.0                ;
acs_TrackPMRA               =    0.0                ;
acs_TrackPMDec              =    0.0                ;
acs_TrackParallax           =    0.0                ;
acs_TrackRadVel             =    0.0                ;
strcpy(str_TrackRotFrame,    ACS_FRAME_CELEST_TXT)  ;
acs_TrackRotFrame           =ACS_FRAME_CELEST       ;
acs_TrackRotAngle           =    0.0                ;
acs_TrackFreq               =  400                  ;
//acs_time_t acs_TrackStart                         
acs_TrackDuration           =    0.0                ;

acs_EnableAG                =false                  ;
acs_EnableMet               =false                  ;
acs_EnableCorr              =false                  ;
acs_EnableOffset            =false                  ;
acs_EnablePMRA              =true                   ;
acs_EnablePMDec             =true                   ;
acs_EnableZD                =true                   ;
acs_EnableAzm               =true                   ;
acs_EnableRot               =true                   ;

strcpy(str_CorrType,         ACS_TYPE_FIXED_TXT)    ;
acs_CorrType                =ACS_TYPE_FIXED         ;
strcpy(str_CorrFrame,        ACS_FRAME_CELEST_TXT)  ;
acs_CorrFrame               =ACS_FRAME_CELEST       ;
acs_CorrX                   =    0.0                ;
acs_CorrY                   =    0.0                ;
acs_CorrRot                 =    0.0                ;

strcpy(str_OffsetType,       ACS_TYPE_FIXED_TXT)    ;
acs_OffsetType              =ACS_TYPE_FIXED         ;
strcpy(str_OffsetFrame,      ACS_FRAME_CELEST_TXT)  ;
acs_OffsetFrame             =ACS_FRAME_CELEST       ;
acs_OffsetX                 =    0.0                ;
acs_OffsetY                 =    0.0                ;
acs_OffsetRot               =    0.0                ;
                                                   
acs_AxisState                                       ;
acs_AxisZD                                          ;
acs_AxisAzm                                         ;
acs_AxisRotAngle                                    ;

strcpy(acs_LogFile,         "acs.log" )             ;
strcpy(str_LogAddrPort,     "127.0.0.1:14555" )     ;
strcpy(acs_LogAddr,         "127.0.0.1" )           ;
acs_LogPort                 =14555                  ;
acs_LogFreq                 =    0.0                ;
strcpy(str_LogAction,        ACS_LOGACTION_GOTO_TXT);
acs_LogAction               =ACS_LOGACTION_GOTO     ;

strcpy(str_ACSNewState,      ACS_STATE_IDLE_TXT)    ;
acs_ACSNewState             =ACS_STATE_IDLE         ;

strcpy(acs_ACSReason,       "STARTUP")              ;
}
