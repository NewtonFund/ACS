/*! ACS main function
 *
 */
#define ACS_MAIN 
#include "acs.h"

/*
 * Command server process main loop (slow) 
*/
int acs_cmd( void )
{
    unsigned char pkt[1024];
    long int len;

    acs_ini_mkmem();
    acs_ini_read();
    acs_ini_data();
    acs_net_cmd_ini( &acs_net_cmd );
    acs_debug( DBG3, "isserver=%i", acs_net_cmd.isserver );

    acs_mem->Ready = true;

    acs_log_msg( true, ACS_PFX_INF, ACS_FAC_ACS, "cmd process starting, pid=%i", acs_cmdpid );
    for(;;)
    {
        len = acs_net_cmd_rcv( &acs_net_cmd, pkt, sizeof(pkt));
        printf("%li", len);
        if ( len > 0 )
        {
            printf( "LEN: %li\n", len );
            acs_jsn_parse( pkt ); 
            puts("Done");
        }
        sleep(1);
    }
}


/*
 * Demand client process main loop (fast) 
*/
int acs_dmd( void )
{
//    unsigned char buf[]="COMMAND";
    acs_pkt_t *dmd_ptr;
    long int dmd_len;
    long int snd_len;

    struct timespec nxt; // Next time to send a packet
    struct timespec now; // The time now 

    acs_ini_mkmem();
    acs_ini_read();
    acs_ini_data();

//  Create a demand packet template
    dmd_len = acs_jsn_mkdmd( &dmd_ptr );

//  Initialise the network interface
    acs_net_dmd_ini( &acs_net_dmd );

    printf( "LEN: %li %s\n", dmd_len, dmd_ptr->jsn ); // DEBUG
  
//  Evaluate the time for the next demand
    clock_gettime( CLOCK_REALTIME, &nxt ); 
    nxt = utl_ts_add( &nxt, &acs_mem->DemandInterval );

    acs_log_msg( true, ACS_PFX_INF, ACS_FAC_ACS, "dmd process starting, pid=%i", acs_dmdpid );
    for(;;)
    {
        puts(".");
//DEBUG:        if ( acs_mem->Ready )
//        {
            acs_ast_calc( &nxt );
            dmd_len = acs_jsn_wrdmd( dmd_ptr );

//          Wait for send time to come around
            do
            {
                clock_gettime( CLOCK_REALTIME, &now ); 
            }
            while ( ( now.tv_sec  < nxt.tv_sec )&&
                    ( now.tv_nsec < nxt.tv_nsec)  ); 

           
            snd_len = acs_net_dmd_snd( &acs_net_dmd, (unsigned char *)dmd_ptr, dmd_len );
            puts(dmd_ptr->jsn);
//            len = acs_net_cli_snd( &acs_net_dmd, buf, sizeof(buf) );
//           nanoleep();
            sleep(1);
//        }
    }
    return ACS_SUCCESS;
}


/*
** Common main loop
*/
int main( int argc, char *argv[] )
{
    p_libsys_init();
    pid_t pid  = getpid();

    acs_dbg_lvl = DBG_LVL;

//  Create sub-process xx*/
//DEBUG:    pid = fork();
    if ( pid > 0 )
    {
//      We are the command process
        acs_cmdpid = getpid();
        acs_dmdpid = pid;
        return acs_log_msg( acs_cmd(), ACS_PFX_SYS, ACS_FAC_ACS, "dmd process exiting" );
    }
    else if ( !pid )
    {
//      We are the demand sub-process
        acs_dmdpid = getpid();
        acs_cmdpid = getppid();
        return acs_log_msg( acs_dmd(), ACS_PFX_SYS, ACS_FAC_ACS, "cmd process exiting" );
    }
    else
    {
//      FATAL: Failed to create sub-process
        perror( ACS_PERROR );
        return acs_log_msg( ACS_FAILURE, ACS_PFX_SYS, ACS_FAC_ACS, "fork() failed, errno=%i", errno ); 
    }

//  This proces should never return 
    return EXIT_FAILURE;
}
