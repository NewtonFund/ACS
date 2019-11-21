/*!
 * Log file functions 
 */

#include "acs.h"


/* Look-up tables to convert numeric to string */ 
char *log_pfx[] = { ACS_PFX_INF_TXT,
                    ACS_PFX_WRN_TXT,
                    ACS_PFX_ERR_TXT,
                    ACS_PFX_LIB_TXT,
                    ACS_PFX_SYS_TXT };
 

char *log_fac[] = { ACS_FAC_ACS_TXT,
                    ACS_FAC_CFG_TXT,
                    ACS_FAC_NET_TXT,
                    ACS_FAC_LOG_TXT,
                    ACS_FAC_MEM_TXT,
                    ACS_FAC_JSN_TXT,
                    ACS_FAC_CHK_TXT };

/*
 * Writes a message to the log file and return a provided value
 * The ret value is for coding convenience to allow single lines 
 */
int acs_log_msg( int ret, int pfx, int fac,  char* fmt, ...)
{
    va_list args;
    FILE   *fp;
    time_t  tnow;
    char    tstamp[80];   

    if ( fp = fopen(ACS_LOG_FILE, "a") )
    {
        time( &tnow );
        strftime( tstamp, sizeof( tstamp ) - 1, ACS_TSTAMP_FMT, localtime( &tnow) );
        va_start( args, fmt );
        fprintf( fp, "%s %s_%s: ", tstamp, log_pfx[pfx], log_fac[fac] );
        vfprintf( fp, fmt, args );
        fputs( "\n", fp );
        va_end( args );
        fclose( fp );
    }
    else
    {
        perror(ACS_LOG_ERR);
    }

//  Can't use acs_debug() macro with va_list so do this long hand   
    if ( acs_dbg_lvl >= DBG3 )
    {
        va_start( args, fmt );
        printf( "DBG: " ); 
        vprintf( fmt, args ); 
        puts("");
        va_end( args );
    }

    return ret;
}


/*!
 * Ordered shutdown of ACS process 
*/
int acs_close( void )
{
   p_libsys_shutdown();

   return acs_log_msg( ACS_SUCCESS, ACS_PFX_INF, ACS_FAC_LOG, "Shutting down" );
}
