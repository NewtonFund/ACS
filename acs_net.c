/*! ACS main function
 *
 */
#include "acs.h"


unsigned char net_txbuf[ACS_NET_PACKET_MAX];
unsigned char net_rxbuf[ACS_NET_PACKET_MAX];

/*
** Free up network resources
*/
void acs_net_free( acs_net_t *net )
{
    if ( net->sktAddr )
    {
        p_socket_address_free( net->sktAddr );
        net->sktAddr = NULL;
    }

    if ( net->sktConn )
    {
        p_socket_close( net->sktConn, &net->sktError );
        p_socket_free( net->sktConn );
        net->sktConn = NULL;
    }
}


/*
 * Check that a network address:port values are within valid limits 
 * If valid the supplied parameters are updated
*/
int acs_net_chk( char *AddrPort, char *Addr, int *Port )
{   
    int a, b, c, d, p; // Numbers making up an IPv4 address:port a.b.c.d:p
    char x;            // Check for any unwanted extra trailing characters 

/*
**  We expect 5 fields but scan for 6x 5 are numeric with '.' or ':' delimiters plus a check for a spurious trailing char 
**  If all expected fields are found check each numeric value is between 0 and 255 inclusive
**  Finally check that the port number is above ICANN reserved and below max
*/  
    if ((5 != sscanf( AddrPort, "%i.%i.%i.%i:%i%c", &a, &b, &c, &d, &p, &x )         )||  // all numeric fields found
        (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255)||  // octets within limits
        (p < 1024 || p > 65535                                                       )  ) // port within limits
    {   
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Invalid IP Address:Port=%s", AddrPort);
    }   
    else
    {
//      Populate arguments if not NULL. NB we assume destination is large enough
        if ( Addr )
            sprintf( Addr, "%i.%i.%i.%i", a,b,c,d ); 

        if ( Port )
           *Port = p;

        return true;
    }
}


/*
 * Demand (serveri mode) network I/F intialisation   
 */
int acs_net_cmd_ini( acs_net_t *net )
{
    net->sktAddr = p_socket_address_new( net->Addr, net->Port );
    net->sktServ = p_socket_new( P_SOCKET_FAMILY_INET,
                              P_SOCKET_TYPE_STREAM,
                              P_SOCKET_PROTOCOL_TCP,
                             &net->sktError );

    p_socket_bind( net->sktServ, net->sktAddr, FALSE, &net->sktError );
    p_socket_set_blocking( net->sktServ, FALSE );
    p_socket_set_keepalive( net->sktServ, FALSE );
//  p_socket_set_timeout( net->sktServ, 100 ); // Timeout in ms used to throttle main loop
    p_socket_listen( net->sktServ, &net->sktError );
    
    return true;
}


/*
 * Command (server mode) network receive
 */
long int acs_net_cmd_rcv( acs_net_t *net, unsigned char *buf, long int siz )
{
    PSocket *Acpt;

//      Wait for new connection 
    if ( Acpt = p_socket_accept( net->sktServ, NULL ) )
    {
//      Release previous connection resources 
        if ( net->sktConn )
            p_socket_free( net->sktConn );
        
        net->sktConn = Acpt;
    }

//  Read from current connection 
    return p_socket_receive( net->sktConn, buf, siz, &net->sktError );
}


/* 
 * Command (server mode) network send
 */
long int acs_net_cmd_snd( acs_net_t *net, unsigned char *buf, long int len )
{
    if ( net->sktConn )
    {
        return p_socket_send( net->sktConn, buf, len, &net->sktError );  
    }
    else
    {
        return -1;
    }
}


/*
 * Demand (client mode) network I/F initialisation 
 */
int acs_net_dmd_ini( acs_net_t *net )
{
//  Create address 
    if ( !net->sktAddr )
        net->sktAddr = p_socket_address_new( net->Addr, net->Port );

//  If connection socket already exists, delete it before re-creating
    if ( net->sktConn )
       p_socket_free( net->sktConn );

    net->sktConn = p_socket_new( P_SOCKET_FAMILY_INET,
                                 P_SOCKET_TYPE_STREAM,
                                 P_SOCKET_PROTOCOL_TCP,
                                &net->sktError );
    p_socket_set_blocking( net->sktConn, FALSE );
    p_socket_connect( net->sktConn, net->sktAddr, &net->sktError );

    return true;
}

/*
 * Demand (client mode) network receive (should never be used) 
 */
long int acs_net_dmd_rcv( acs_net_t *net, unsigned char *buf, long int siz )
{
    return p_socket_receive( net->sktConn, buf, siz, &net->sktError ); 
}


/*
 * Demand (client mode) network send (used a lot)
 */
long int acs_net_dmd_snd( acs_net_t *net, unsigned char *buf, long int len )
{
    long n;

    n = p_socket_send( net->sktConn, buf, len , &net->sktError ); 
    if ( n < 0L )
        acs_net_dmd_ini( net );

    return n;
}

