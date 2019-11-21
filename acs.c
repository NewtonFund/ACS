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

    acs_log_msg( true, ACS_PFX_INF, ACS_FAC_ACS, "cmd process starting, pid=%i", acs_cmdpid );

    acs_ini_mkmem2();
    acs_ini_read();
    acs_ini_data();
    acs_net_cmd_ini( &acs_net_cmd );
    acs_debug( DBG3, "isserver=%i", acs_net_cmd.isserver );

    acs_mem->Ready = false; // Debug

    for(;;)
    {
        len = acs_net_cmd_rcv( &acs_net_cmd, pkt, sizeof(pkt));
//        printf("%li", len);
        if ( len > 0 )
        {
//            printf( "LEN: %li\n", len );
            acs_jsn_parse( pkt ); 
//            puts("Done");
        }
        sleep(1);
    }
}


/*
 * Demand client process main loop (fast) 
*/
int acs_dmd( void )
{
    acs_pkt_t *dmd_ptr;
    long int dmd_len;
    long int snd_len;

    struct timespec nxt; // Next time to send a packet
    struct timespec now; // The time now 

    acs_log_msg( true, ACS_PFX_INF, ACS_FAC_ACS, "dmd process starting, pid=%i", acs_dmdpid );

    acs_ini_mkmem2();
    acs_ini_read();
    acs_ini_data();

//  Create a demand packet template
    dmd_len = acs_jsn_mkdmd( &dmd_ptr );

//  Initialise the network interface
    acs_net_dmd_ini( &acs_net_dmd );

    for(;;)
    {
        if ( acs_mem->Ready )
        {
//          Evaluate the time for the next demand
            clock_gettime( CLOCK_REALTIME, &nxt ); 
            nxt = utl_ts_add( &nxt, &acs_mem->DemandInterval );
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
//            puts(dmd_ptr->jsn);
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
    int o; // Command line option

//  Set default debug level 
    acs_dbg_lvl = DBG_DEFAULT;

//  Parse for command line options
    opterr = 0;
    while( (o = getopt( argc, argv, "hd:c:")) != -1 )
    {
        switch(o)
        {
           case 'h':
              printf("Usage: %s [OPTION]\n", argv[0]);
              puts("Astronometric Conversion Service");
              puts("");
              printf(" -c specify configuration file\n" );
              printf(" -d set debug level 0-3\n" );
              printf(" -h display this help\n" );
              puts("");
              exit( EXIT_SUCCESS );
              break;

           case 'c':
              if ( utl_filexist( optarg ) ) 
                 acs_log_msg( ACS_SUCCESS, ACS_PFX_INF, ACS_FAC_CFG, "Config. file %s not found", optarg ); 
              else
                 fprintf( stderr, "Config file '%s' not found\n", optarg );
                 exit( EXIT_FAILURE );
              break;

           case 'd':
              acs_dbg_lvl = atoi(optarg);
//            Clip requested debug level
              if (acs_dbg_lvl < DBG0)
                  acs_dbg_lvl = DBG0;
              if (acs_dbg_lvl > DBG3)
                  acs_dbg_lvl = DBG3;
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

//  Create sub-process */
    pid = fork();
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
        perror( ACS_SUB_ERR );
        exit( acs_log_msg( EXIT_FAILURE, ACS_PFX_SYS, ACS_FAC_ACS, "fork() failed, errno=%i", errno ) ); 
    }

//  This process should never return 
    return EXIT_FAILURE;
}
