/*! ACS main function
 *
 */

#define ACS_MAIN 
#include "acs.h"
#include "acs_dat.h"

/*
 * Initialise global variables
*/
int acs_ini_mem(void)
{
    PError *error; 

    acs_dbg_lvl = 0;
    const PShm *shm;
   
   if (( shm     = p_shm_new( ACS_TXT_MEM, sizeof( acs_mem_t ), P_SHM_ACCESS_READWRITE, &error ))&&
       ( acs_mem = p_shm_get_address( shm )                                                     )  ) 
    {
        memset( acs_mem, 0, sizeof( acs_mem_t ) );
        return ACS_SUCCESS;
    }
    else
    {
        acs_log_msg( ACS_PFX_SYS, ACS_FAC_ACS, "p_shm_new() Failed, acs_mem=%p", acs_mem ); 
        return ACS_FAILURE;
    }
}

/*
 * Command process main loop 
*/
int acs_cmd(void)
{
    unsigned char buf[1024];
    long int len;
    
    acs_ini_mem();
    acs_ini_read();
    acs_ini_data();
    acs_net_open( acs_IPType, acs_IPAddress, acs_IPPort );
    for(;;)
    {
        acs_net_read( buf, sizeof(buf), &len);
        sleep(1);
        puts(".");
    }
}


/*
 * Demand process main loop 
*/
int acs_dmd()
{
    acs_debug( DBG3, "Dmd: cmdpid=%i, dmdpid=%i", acs_cmdpid, acs_dmdpid ); 
    acs_ini_mem();
    for(;;)
    {
        if ( acs_mem->Ready )
        {
            acs_ast_calc();
        }
    }
    return ACS_SUCCESS;
}


/*
 * Common main loop
*/
int main( int argc, char *argv[] )
{
    p_libsys_init();
    pid_t pid;

    acs_dbg_lvl = DBG_LVL;

/*  Create sub-process */
    pid = fork();
    if ( pid > 0 )
    {
/*      We are the command process */
        acs_cmdpid = getpid();
        acs_dmdpid = pid;
        return acs_cmd();       
    }
    else if ( pid == 0)
    {
/*      We are the demand sub-process */
        acs_dmdpid = getpid();
        acs_cmdpid = getppid();
        return acs_dmd();       
    }
    else
    {
        acs_log_msg( ACS_PFX_SYS, ACS_FAC_ACS, "fork() failed, errno=%i", errno ); 
        perror( ACS_PERROR );
    }

    return EXIT_FAILURE;
}
