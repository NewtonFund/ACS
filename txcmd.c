/*!
 * Test utility to send a command to the ACS
 *   Input is a *.cmd file containing tag/data to be sent
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "plibsys.h"

static PSocketAddress *addr = NULL;
static PSocket        *conn = NULL; 

void net_ini( void )
{
    if ( !addr )
        addr = p_socket_address_new( "127.0.0.1", 14111 );

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

//  Read in the test message
    while ( EOF != ( c = fgetc( fp ) ) )
    {
       tx_buf[tx_len++] = c;
    }
    tx_buf[--tx_len]='\0';

    p_libsys_init();
    net_ini();

//  Send the packet
    if ( tx_len = p_socket_send( conn, tx_buf, tx_len , &error ) < 0 )
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
