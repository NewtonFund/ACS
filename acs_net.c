/*! ACS main function
 *
 */
#define ACS_MAIN 
#include "acs.h"
#include "acs_dat.h"

PSocket        *acs_conn = NULL;
PSocket        *acs_sock = NULL;
PSocketAddress *acs_addr = NULL;

unsigned char acs_tx_buf[ACS_NET_PACKET_MAX];
unsigned char acs_tx_buf[ACS_NET_PACKET_MAX];

/*
 * Initialise inbound socket 
*/
int acs_net_open( char *IPType, char *IPAddress, int IPPort )
{
    PError *error;

/*  Create new address:port */
    if (( acs_addr = p_socket_address_new( IPAddress, IPPort )) == NULL )
    {
        acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_address_new() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
        return ACS_FAILURE; 
    }

/*  Create socket of the appropriate type */
    if ( !strcmp( acs_IPType, "TCP" ) )
    {
        if (( acs_sock = p_socket_new (P_SOCKET_FAMILY_INET,
                                       P_SOCKET_TYPE_STREAM,
                                       P_SOCKET_PROTOCOL_TCP,
                                      &error)) == NULL)
        {
            acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_new() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
            p_socket_address_free( acs_addr );
            return ACS_FAILURE; 
        }

/*      Bind to socket */ 
        if (!p_socket_bind(acs_sock, acs_addr, TRUE, &error))
        {
            acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_bind() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
            p_socket_address_free( acs_addr );
            p_socket_free( acs_sock);
            return ACS_FAILURE; 
        }

/*      Disable blocking to allow main loop to periodically check for messages */
        p_socket_set_blocking( acs_sock, FALSE );
//      p_socket_set_timeout( acs_sock, 100 ); // Timeout in ms used to throttle main loop

/*      Start listending */
        if (!p_socket_listen( acs_sock, &error ))
        {
            acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_listen() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
            p_socket_address_free( acs_addr );
            p_socket_free( acs_sock );
            return ACS_FAILURE;
        }

        return ACS_SUCCESS;
    }
    else if (!strcmp( acs_IPType, "UDP" ))
    {
        if (( acs_sock = p_socket_new( P_SOCKET_FAMILY_INET,
                                       P_SOCKET_TYPE_DATAGRAM,
                                       P_SOCKET_PROTOCOL_UDP,
                                       &error )) == NULL )
        {
            acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_new() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
            p_socket_address_free( acs_addr );
            return ACS_FAILURE; 
        }

        if (!p_socket_bind( acs_sock, acs_addr, TRUE, &error ))
        {
            acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "%s p_socket_bind() failed for IPAddress=%s:%i", IPType, IPAddress, IPPort ); 
            p_socket_address_free( acs_addr );
            p_socket_free( acs_sock );
            return ACS_FAILURE; 
        }

/*      Disable blocking to allow main loop to periodically check for messages */
        p_socket_set_blocking( acs_sock, FALSE );
//      p_socket_set_timeout( acs_sock, 100 ); // Timeout in ms used to throttle main loop

        return ACS_SUCCESS;
    }
    else /* Unknown protocol */
    {
        acs_log_msg( ACS_PFX_SYS, ACS_FAC_NET, "Unrecognised protocol %s", acs_IPType ); 
        return ACS_FAILURE;
    }
}


/*
 *
 */
int acs_net_read( unsigned char *buf, int max, long int *len )
{
    PError *error;

    if ( acs_conn = p_socket_accept( acs_sock, &error ) )
    {
        if ( *len = p_socket_receive( acs_conn, buf, max, &error ) )
        {
           printf("Rx len=%li:%s\n", *len, buf );
        }
    }
}

/*
 *
 */
int acs_net_write( unsigned char *buf, int len )
{
    PError *error;

    return p_socket_send( acs_conn, buf, len, &error );
}


/*
 * Shutdown network port(s)
 */
int acs_net_close( void )
{
    PError *error;

    p_socket_address_free( acs_addr );
    p_socket_close( acs_sock, &error );
    p_socket_free( acs_sock );
}
