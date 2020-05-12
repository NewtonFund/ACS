/*! 
 * ACS Main function
 */
#define ACS_MAIN 
#include "acs.h"
#define FAC ACS_FAC_ACS


/*
 * Command server process main loop (slow) 
 */
int acs_cmd( void )
{
    char          *ack; // JSON ACK data 
    acs_pkt_t     *pkt; // Received packet
    unsigned char *jsn; // Received JSON data from packet

    log_msg( true, LOG_INF, FAC, "cmd process starting, pid=%i", acs_cmdpid );

    ini_mkmem();
    ini_read();
    ini_data();
    net_cmd_ini( &acs_net_cmd );

    log_msg( true, LOG_INF, FAC, "cmd process starting, pid=%i", acs_cmdpid );
    log_msg( true, LOG_DBG, FAC, "isserver=%i", acs_net_cmd.isserver );

    acs_mem->Ready = false; // DEBUG 

    for(;;) // Forever receive, parse and reply 
    {
        if ( pkt = net_cmd_rcv( &acs_net_cmd) )
        {
            log_dmp_pkt( pkt );
            if ( ack = net_hdr_parse( pkt ) ) 
                net_ack_snd( &acs_net_cmd, pkt, ack );
        }
    }
}


/*
 * Demand client process main loop (fast) 
 */
int acs_dmd( void )
{
    acs_pkt_t *dmd_ptr;
    long dmd_len;
    long snd_len;

    struct timespec nxt; // Next time to send a packet
    struct timespec now; // The time now 

//  This process is not in command, it sends the demands
    acs_in_cmd = false;

    log_msg( true, LOG_INF, FAC, "dmd process starting, pid=%i", acs_dmdpid );
    log_msg( true, LOG_DBG, FAC, "isserver=%i", acs_net_cmd.isserver );

    ini_mkmem();
    ini_read();
    ini_data();

//  Create demand packet template
    dmd_len = jsn_mkdmd( &dmd_ptr );

//  Initialise the network interface
    net_dmd_ini( &acs_net_dmd );

    for(;;)
    {
        if ( acs_mem->Ready )
        {
//          Evaluate the time for the next demand
            clock_gettime( CLOCK_REALTIME, &nxt ); 
            nxt = utl_ts_add( &nxt, &acs_mem->DemandInterval );
            acs_ast_calc( &nxt );
            dmd_len = jsn_wrdmd( dmd_ptr );

//          Wait for send time to come around
            do
            {
                clock_gettime( CLOCK_REALTIME, &now ); 
            }
            while ( ( now.tv_sec  < nxt.tv_sec )&&
                    ( now.tv_nsec < nxt.tv_nsec)  ); 
           
            snd_len = net_dmd_snd( &acs_net_dmd, dmd_ptr, dmd_len );
//           puts(dmd_ptr->jsn);
//           nanoleep();
            sleep(1);
        }
        else
        { 
//          Not ready so wait a sec
            sleep(1);
        }
    }
    return ACS_SUCCESS;
}


/*
** Common main loop
*/
int main( int argc, char *argv[] )
{
    int opt; // Command line option

//  This proces is in command
    acs_in_cmd = true;

//  Parse for command line options
    opterr = 0;
    while( (opt = getopt( argc, argv, "hd:c:")) != -1 )
    {
        switch(opt)
        {
           case 'h':
              printf("Usage: %s [OPTION]\n", argv[0]);
              puts("Astronometric Conversion Service");
              puts("");
              printf(" -c specify configuration file\n" );
              printf(" -d set debug level 0-5\n" );
              printf(" -h display help\n" );
              puts("");
              exit( EXIT_SUCCESS );
              break;

           case 'c':
              if ( utl_filexist( optarg ) ) 
                 log_msg( ACS_SUCCESS, LOG_INF, FAC, "Config. file %s not found", optarg ); 
              else
                 fprintf( stderr, "Config file '%s' not found\n", optarg );
                 exit( EXIT_FAILURE );
              break;

           case 'd':
              acs_log_lvl = atoi(optarg);
              break;

           case '?':
              if ( strchr("hcd", optopt ))
                 fprintf( stderr, "Option - %c missing argument\n", optopt );
              else if  (isprint(optopt))
                 fprintf( stderr, "Option - %c unrecognised\n", optopt );
              else if  (isprint(optopt))
                 fprintf( stderr, "Invalid option -0x%02x character\n", optopt );
              break;

           default:
              exit( EXIT_FAILURE );
        }
    }

//  Initialise plibsys
    p_libsys_init();
    pid_t pid  = getpid();

//  Create sub-process 
    pid = fork();
    if ( pid > 0 )
    {
//      We are the command process
        acs_cmdpid = getpid();
        acs_dmdpid = pid;
        return log_msg( acs_cmd(), LOG_SYS, FAC, "dmd process exiting" );
    }
    else if ( !pid )
    {
//      We are the demand sub-process
        acs_dmdpid = getpid();
        acs_cmdpid = getppid();
        return log_msg( acs_dmd(), LOG_SYS, FAC, "cmd process exiting" );
    }
    else
    {
//      FATAL: Failed to create sub-process
        perror( ACS_SUB_ERR );
        exit( log_msg( EXIT_FAILURE, LOG_SYS, FAC, "fork() failed, errno=%i", errno ) ); 
    }

//  This process should never return 
    return EXIT_FAILURE;
}
