/*!
 * Test utility to send a command to the ACS
 *   Input is a *.cmd file containing tag/data to be sent
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <plibsys.h>
#include "acs.h"

static PSocketAddress *addr = NULL;
static PSocket        *conn = NULL; 

void net_ini( void )
{
    if ( !addr )
        addr = p_socket_address_new( "127.0.0.1", 9001 );

    if ( conn )
       p_socket_free( conn );

    conn = p_socket_new( P_SOCKET_FAMILY_INET,
                         P_SOCKET_TYPE_STREAM,
                         P_SOCKET_PROTOCOL_TCP,
                         NULL );
    p_socket_set_blocking( conn, FALSE );
    p_socket_connect( conn, addr, NULL );
}


/*
 * Main  
*/
int main( int argc, char *argv[] )
{
    PError *error;

    unsigned char tx_buf[2048];
    unsigned char rx_buf[1024];
    long int tx_len=0;
    long int rx_len=0;
    int c;
    
    acs_pkt_t pkt;

    FILE *fp;

//  Must have a filename
    if ( argc != 2 )
    {
        puts("Provide a file name to send to ACS" );
        return EXIT_FAILURE;
    }

//  Try and open file
    if (!(fp = fopen( argv[1], "r" )))
    {
        printf( "File not found FIL=%s", argv[1] );
        return EXIT_FAILURE;
    }

    tx_len=sprintf( pkt.jsn, "{\"NODEID\":15,\"IPADDRESSPORT\":\"127.0.0.1:8081\",\"COMMANDSTR\":"); 

//  Read in the test message
    while ( EOF != ( c = fgetc( fp ) ) )
    {
       pkt.jsn[tx_len++] = c;
    }
    pkt.jsn[--tx_len]='}';
    pkt.jsn[++tx_len]='\0';

    pkt.hdr.start  = 25510027;
    pkt.hdr.ID     = 1;
    pkt.hdr.count  = 1;
    pkt.hdr.number = 1;
    pkt.hdr.jsn_len= tx_len;
    pkt.hdr.ack    = 1;
    pkt.hdr.type   = 2;
    pkt.hdr.timestamp  = utl_ts2ad( TIME_NOW ) ;

    tx_len += ACS_HDR_SIZEOF; 
    printf("TXPKT: txlen=%li\n", tx_len );

    p_libsys_init();
    net_ini();

//  Send the packet
    if ( tx_len = p_socket_send( conn, (unsigned char *)&pkt, tx_len , &error ) < 0 )
    { 
        printf("TXPKT: Fail txlen=%li\n", tx_len );
        return EXIT_FAILURE;
        net_ini();
    }
    else
    {
        printf( "TXPKT: OK txlen=%li\n", tx_len );
    }


//    printf( "TXPKT: Awaiting Reply\n" );
    for(;;)
    {
        if ( rx_len = p_socket_receive( conn, rx_buf, sizeof(rx_buf), &error ) )
        {
            rx_buf[rx_len] = '\0';
            printf( "TXPKT: rxlen=%li=%s\n", rx_len, rx_buf );
        }    
        sleep(1);        
        printf( "%s", "." );
    }

    return EXIT_SUCCESS;
}
