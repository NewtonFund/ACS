/*!
 * ACS Network functions
 */
#include "acs.h"
#define FAC ACS_FAC_NET

// Local data
unsigned char net_txbuf[ACS_NET_PACKET_MAX];
unsigned char net_rxbuf[ACS_NET_PACKET_MAX];


/*
 * Free up network resources
 */
void net_free( acs_net_t *net )
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
int net_chk_addr( char *addrport, char *addr, int *port )
{   
    int a, b, c, d, p; // Numbers making up an IPv4 address:port a.b.c.d:p
    char x;            // Check for any unwanted extra trailing characters 

//  We expect 5 fields but scan for 6. 5 are numeric with '.' or ':' delimiters plus a check for a spurious trailing char 
//  If all expected fields are found check each numeric value is between 0 and 255 inclusive
//  Finally check that the port number is above ICANN reserved set and below max
    if ((5 != sscanf( addrport, "%i.%i.%i.%i:%i%c", &a, &b, &c, &d, &p, &x )         )||  // Wrong No of fields
        (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255)||  // Octets out-of-range
        (p < 1024 || p > 65535                                                       )  ) // Port No. out-of-range
    {   
        return log_msg( false, LOG_WRN, FAC, "Invalid IP Address:Port=%s", addrport);
    }   
    else
    {
//      Populate arguments if not NULL. NB we assume destination is large enough
        if ( addr )
            sprintf( addr, "%i.%i.%i.%i", a,b,c,d ); 

        if ( port )
           *port = p;

        return true;
    }
}


/*
 * Demand (server mode) network I/F initialisation   
 */
int net_cmd_ini( acs_net_t *net )
{
    net->sktAddr = p_socket_address_new( net->Addr, net->Port );
    net->sktServ = p_socket_new( P_SOCKET_FAMILY_INET,
                                 P_SOCKET_TYPE_STREAM,
                                 P_SOCKET_PROTOCOL_TCP,
                                &net->sktError );

    p_socket_bind( net->sktServ, net->sktAddr, FALSE, &net->sktError );
    p_socket_set_blocking ( net->sktServ, FALSE );
    p_socket_set_keepalive( net->sktServ, TRUE );
 //   p_socket_set_timeout( net->sktServ, 100 ); // Timeout in ms used to throttle main loop
    p_socket_listen( net->sktServ, &net->sktError );
    
    return true;
}


/*
 * Command (server mode) network receive
 */
acs_pkt_t *net_cmd_rcv( acs_net_t *net )
{
    PSocket *Acpt;

    static bool start = true;
    static int  stall = 0;
    static int  idx = 0;
    static long len = 0;
    static long min = ACS_HDR_SIZEOF;         // Minimum read at start
    static unsigned char pkt[ACS_PKT_SIZEOF]; // Rx packet
    static acs_pkt_t    *ptr = (void *)pkt;

//  Wait for new connection 
    if ( Acpt = p_socket_accept( net->sktServ, NULL ) )
    {
//      Release previous connection resources 
        if ( net->sktConn )
            p_socket_free( net->sktConn );
        
        net->sktConn = Acpt;
    }

//  Read from current connection
    len = p_socket_receive( net->sktConn, &pkt[idx], min, &net->sktError );
    if ( len <= 0 ) // Zero length read 
    {
//        log_msg( false, LOG_DBG, FAC, "PKT: stall=%i", stall );
        if ( stall && !--stall ) // Is stall detection active and about to go off?
        {
            log_msg( ACS_ERROR, LOG_ERR, FAC, "RCV stall, %li bytes missing", min );
            start = true;
            idx   = 0;
            min   = ACS_HDR_SIZEOF;
        }

    }
    else if ( start )
    {
        log_msg( false, LOG_DBG, FAC, "PKT: hdr frag. len=%li", len );
        stall = 16; // No. of zero length reads before giving up
        idx += len; // Increment buffer index
        min -= len; // Decrement read request
        if ( min <= 0 )
        {
            // Sanity check. Will bytes fit into buffer?
            if ( ptr->hdr.jsn_len > ACS_JSN_SIZEOF )
            {
                log_msg( ACS_ERROR, LOG_ERR, FAC, "RCV length %i too large", ptr->hdr.jsn_len );
                p_socket_free( net->sktConn );
                start = true;
                idx   = 0;
                min   = ACS_HDR_SIZEOF;
            }
            else
            {
                min = ptr->hdr.jsn_len;
                start = false;
            }
        }
    }
    else // Received header, how many more bytes?
    {
        log_msg( false, LOG_DBG, FAC, "PKT: jsn frag. len=%li", len );
        idx += len;
        min -= len;  
        if ( min <= 0 )
        {
            start = true;
            stall = 0;
            idx   = 0;
            min   = ACS_HDR_SIZEOF; 
            return ptr;  
        }
    }   

    return NULL;  // Return NULL until full packet received
}


/* 
 * Command (server mode) network send
 */
long net_cmd_snd( acs_net_t *net, acs_pkt_t *pkt, long pkt_len )
{
    if ( net->sktConn )
    {
        return p_socket_send( net->sktConn, (unsigned char *)pkt, pkt_len, &net->sktError );  
    }
    else
    {
        return -1;
    }
}


/*
 * Demand (client mode) network I/F initialisation 
 */
int net_dmd_ini( acs_net_t *net )
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
long net_dmd_rcv( acs_net_t *net, acs_pkt_t *pkt, long pkt_len )
{
    return p_socket_receive( net->sktConn, (unsigned char *)pkt, pkt_len, &net->sktError ); 
}


/*
 * Demand (client mode) network send (used a lot)
 */
long net_dmd_snd( acs_net_t *net, acs_pkt_t *pkt, long pkt_len )
{
    long n;

    n = p_socket_send( net->sktConn, (unsigned char*)pkt, pkt_len , &net->sktError ); 
    if ( n < 0L )
        net_dmd_ini( net );

    return n;
}


/*
 * Validate and parse packet header and invoke commands 
 */
char *net_hdr_parse( acs_pkt_t *pkt )
{
    unsigned char *jsn = pkt->jsn; // Pointer to JSON data

    if ( pkt->hdr.start  != acs_StartPackage )
        log_msg( ACS_ERROR, LOG_ERR, FAC, "Package Start: Expected=%i Received=%i", acs_StartPackage, pkt->hdr.start );

    if ( pkt->hdr.count  != 1 )
        log_msg( ACS_ERROR, LOG_ERR, FAC, "Package count: Expected=1 Received=%i", pkt->hdr.count );

    if ( pkt->hdr.number != 1 )
        log_msg( ACS_ERROR, LOG_ERR, FAC, "Package number: Expected=1 Received=%i", pkt->hdr.number );

    switch ( pkt->hdr.type )
    {
        case ACS_HDR_TYPE_GET:
           break;  
        case ACS_HDR_TYPE_SET:
	    if ( utl_unchar( jsn, " \n\r\\"           )&&
             utl_upcase( jsn                      )&&
	         jsn_parse (&jsn, ACS_TAG_NODEID      )&&
	         jsn_parse (&jsn, ACS_TAG_COMMANDNAME )&&
	         pkt->hdr.ack                            )
	    {
            return "";            // Success. No error
        }
	    else if ( pkt->hdr.ack )  // Fail. Supply error
        {
           return "{\"Error\":\"Command failed\"}"; 
	    }
	    break;  
        case ACS_HDR_TYPE_SUBSCRIBE:
            break;  
        case ACS_HDR_TYPE_CALLBACK:
            break;  
        case ACS_HDR_TYPE_ACK:
            break;  
        case ACS_HDR_TYPE_ERROR:
            break;  
        default:
            break;  
    } 

    return NULL;
}


/*
 * Send acknowledgement  
 */
long net_ack_snd( acs_net_t *net, acs_pkt_t *pkt, char *jsn )
{
    acs_pkt_t ack;
    long snd_len = ACS_HDR_SIZEOF;

    ack.hdr.start  = pkt->hdr.start;
    ack.hdr.ID     = pkt->hdr.ID;
    ack.hdr.count  = 1;
    ack.hdr.number = 1;
    ack.hdr.ack    = 0; 

    if ( jsn )
    {
        ack.hdr.type = ACS_HDR_TYPE_ERROR;  
        ack.hdr.jsn_len = strlen( jsn );
        strcpy( ack.jsn, jsn );
        snd_len += ack.hdr.jsn_len;
    }
    else
    {
        ack.hdr.type = ACS_HDR_TYPE_ACK;  
        ack.hdr.jsn_len = 0;
    }
    return net_cmd_snd( net, &ack, snd_len );
}
