/*!
 * ACS Logging and debug functions 
 */

#include "acs.h"
#define FAC ACS_FAC_LOG

/* Look-up tables to convert numeric to string */ 
char *log_pfx[] = { LOG_INF_TXT,
                    LOG_WRN_TXT,
                    LOG_ERR_TXT,
                    LOG_LIB_TXT,
                    LOG_SYS_TXT,
                    LOG_DBG_TXT };
 

char *log_fac[] = { ACS_FAC_ACS_TXT,
                    ACS_FAC_AST_TXT,
                    ACS_FAC_CHK_TXT,
                    ACS_FAC_CMD_TXT,
                    ACS_FAC_INI_TXT,
                    ACS_FAC_JSN_TXT,
                    ACS_FAC_LOG_TXT,
                    ACS_FAC_NET_TXT,
                    ACS_FAC_UTL_TXT };

/*
 * Writes a message to the log file and return a provided value
 * The ret value is for coding convenience to allow single lines 
 */
int log_msg( int ret, int lvl, int fac, char* fmt, ...)
{
    va_list args;

    if ( lvl <= acs_log_lvl )
    {
        va_start( args, fmt );
        printf(  "%s %s%i %s: ", utl_ts2str( TIME_NOW, NULL ), log_pfx[lvl], acs_in_cmd, log_fac[fac] );
        vprintf( fmt, args );
        puts("");
        va_end( args );
    }

    return ret;
}


/*
 * Ordered shutdown of ACS process 
 */
int acs_close( void )
{
    p_libsys_shutdown();

    return log_msg( ACS_SUCCESS, LOG_INF, FAC, "Shutting down" );
}


/*
 * DEBUG: Dump received packet 
 */
void log_dmp_pkt( acs_pkt_t *pkt )
{
    unsigned char *ptr = (void *)pkt;

    printf( "HDR: Start package =%i\n" , pkt->hdr.start    );
    printf( "     Package ID    =%li\n", pkt->hdr.ID       );
    printf( "     Package Count =%i\n" , pkt->hdr.count    );
    printf( "     Package Number=%i\n" , pkt->hdr.number   );
    printf( "     JSON Length   =%i\n" , pkt->hdr.jsn_len  );
    printf( "     ACK State     =%i\n" , pkt->hdr.ack      );
    printf( "     Command Type  =%i\n" , pkt->hdr.type     );
    printf( "     Timestatmp    =%li\n", pkt->hdr.timestamp);
    printf( "LEN: %i\n", strlen(pkt->jsn) );
    printf( "JSN: %s\n", pkt->jsn );
}	
