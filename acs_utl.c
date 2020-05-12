/*!
 * ACS Utility functions
 */
#include "acs.h"
#define FAC ACS_FAC_UTL


/*
 * Removed selected characters from a string
 * Return - length of string
 */

int utl_unchar( char *str, char *chars )
{
    char *src = str;
    char *dst = str;

    while( *src )
        if ( strchr( chars, *src ))
            src++;
	else
            *dst++ = *src++;
    *dst = '\0';

    return dst - str;
}


/*
 * Substitute a character in a string
 * Return length of string
 */
int utl_subchar( char *str, char old, char new )
{
    char *ptr = str;

    while( *ptr )
        if ( *ptr == old )
            *ptr++ = new;

    return ptr - str;
}


/*
 * Uppercase a string
 * Return length of string
 */
int utl_upcase( char *str )
{
    char *ptr = str;

    for( ; *ptr; ptr++ )
        if ( isalpha( *ptr ) ) 
	    *ptr &= 0xDF; 

    return ptr - str;
}


/*
 * Convert backslash escaped quotes to normal quotes inplace
 * Return length of string
 */
int utl_unslash( char *str )
{
   char *src = str;
   char *dst = str;

    while ( *src )
    {
        if ( *src == '\\' && *(src+1) == '\"' )
            src++;
        *dst++ = *src++;
    }
    *dst = '\0';

    return dst - str;
}


/* 
 * Check if file exists 
 *
 */ 
int utl_filexist( char *file )
{
   struct stat st;

   return ( stat( file, &st ) == 0 );
} 


/*
 * Convert text to an index value used as a pseudo enumeration
 * This is dumb sequential search only suitable for small tables
 */
int utl_val2enum( unsigned char *val, unsigned char *tbl[] )
{
    int i = 0;

//  End of table is marked with a nul.
    while( *tbl[i] )
        if (!strcmp( tbl[i++], val ))
            return ++i; // Add 1 to avoid using 0 

    return ACS_ENUM_NOT_FOUND;
}


/*
 * Generate a Microsoft timestamp in 100ns ticks since start of year 1 AD
 */
char *utl_ts2adstr( struct timespec *ts_ptr, char *str )
{
    struct      timespec ts;
    static char timestr[32];

//  If no timespec supplied use local storage and get now
    if ( !ts_ptr )
    {
        ts_ptr = &ts;
        clock_gettime( CLOCK_REALTIME, ts_ptr ); 
    }

//  If no string destination the use static storage
    if ( !str )
        str = timestr; 

//  Print string 
    snprintf(str, sizeof(timestr-1 ), "%li", utl_ts2ad( ts_ptr ));

    return str; 
}


/*
** Convert a double to a timespec
*/
struct timespec utl_dbl2ts( double dlb )
{
    struct timespec ts;

    ts.tv_sec  = (int)dlb;
    ts.tv_nsec = (dlb - ts.tv_sec) * NANOSECOND; 

    return ts; 
}


/*
 * Add timespec time values and return sum t1 + t2
 */
struct timespec utl_ts_add( struct timespec *t1, struct timespec *t2 )
{
    struct timespec sum;

    sum.tv_sec  = t1->tv_sec  + t2->tv_sec;
    sum.tv_nsec = t1->tv_nsec + t2->tv_nsec;

//  If tv_nsec overflowed apply correction
    if ( sum.tv_nsec >= NANOSECOND )
    {
       sum.tv_sec++;
       sum.tv_nsec -= NANOSECOND; 
    }  

    return sum;
} 


/*
 * Subtract timespec time values and return t1 - t2 
 */
struct timespec utl_ts_sub( struct timespec *t1, struct timespec *t2 )
{
    struct timespec diff = {0, 0};

    if (( t1->tv_sec  < t2->tv_sec                                 )||
        ((t1->tv_sec == t2->tv_sec) && (t1->tv_nsec <= t2->tv_nsec))  )
    {
//      Invalid or identical subtraction. Return {0,0};
        return diff;
    }
    else
    {
        diff.tv_sec = t1->tv_sec - t2->tv_sec;
        if ( t1->tv_nsec <  t2->tv_nsec )
        {
            diff.tv_nsec = NANOSECOND + t1->tv_nsec - t2->tv_nsec;
            diff.tv_sec--;
        }
        else
        {
            diff.tv_nsec = t1->tv_nsec - t2->tv_nsec;
        }
    }

    return diff;
}


/*
 * Compare timespec time values T1 and T2
 * Returns:
 *         -1 : T1 < T2
 *          0 : T1 = T2
 *          1 : T1 > T2
 */
int utl_ts_cmp( struct timespec *t1, struct timespec *t2 )
{
    if      ( t1->tv_sec  < t2->tv_sec )  
        return -1;                     // Less than
    else if ( t1->tv_sec  > t2->tv_sec )
        return  1;                     // Greater than
    else if ( t1->tv_nsec < t2->tv_nsec)
        return -1;                     // Less than
    else if ( t1->tv_nsec > t2->tv_nsec)
        return  1;                     // Greater than
    else
        return  0;                     // Equal to 
}


/*
 * Convert Windows AD 1 epoch to C timespec structure
 */
struct timespec utl_ad2ts( long ad )
{
    int    len;
    char   str[64];
    struct timespec ts;

//  Print Windows 1 AD timestamp    
    len = sprintf( str, "%li", ad );

//  Convert fractional part to 100ns resolution
    ts.tv_nsec = atol( &str[len-7] ) * 100L;        

//  Add nul terminator at start of fractional part
    str[7] = '\0';

//  Adjust whole seconds since 1 AD to Unix 1970 epoch    
    ts.tv_sec  = atol( str ) - SECONDS1970; 

    return ts;
}



/*
 * Convert C Unix timespec to Windows AD 1 epoch 
 */
long utl_ts2ad( struct timespec *ts_ptr )
{
    struct timespec ts;

//  If no time supplied use now
    if (!ts_ptr)
    {
        ts_ptr = &ts; 
        clock_gettime( CLOCK_REALTIME, ts_ptr );
    }

    return (SECONDS1970 + ts_ptr->tv_sec) * 10000000.0 + ts_ptr->tv_nsec / 100.0;
}


/*
 * Convert Unix timespec to ACS standard timestamp string
 * If no time provided use now
 */
char *utl_ts2str( struct timespec *ts_ptr, char *str )
{
    struct timespec ts;
    static char buf[32];   

    if ( !ts_ptr )
    {
//      If no time provided use now
        clock_gettime( CLOCK_REALTIME, &ts );
    }
    else
    {
        // Copy supplied value because we may modify it
        ts = *ts_ptr;
    }

    if (! str )
        str = buf; 

//  Handle rounding to 3 decimal places
    if (ts.tv_nsec >= 999500000)
    {
        ts.tv_sec++;             // Round up integer second
        ts.tv_nsec -= 999500000; // Round down fraction
    }

//  This bit is messy and uses magic numbers.
//  First print fractional second at the end eg 0.123 
//  Then print date/time form start but which overwrites 0. of fraction
//  Finally restore the decimal point at index 19
//  01234567890123456789012
//  YYYY-MM-DDThh:mm:ss.sss
//  <-----------------XX--> XX shows overlap
    snprintf( &str[18], 6, "%.3f", ts.tv_nsec/(double)NANOSECOND ) ;
    strftime( str, 20, "%Y-%m-%dT%H:%M:%S", localtime(&ts.tv_sec) );
    str[19] = '.';

//  Return the rounded version
    return str;
}
