/*! ACS main function
 *
 */
#include "acs.h"


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
            return ++i; // Add 1 to avoid using 

    return ACS_ENUM_NOT_FOUND;
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
    if ( sum.tv_nsec >= NANOSECONDS )
    {
       sum.tv_sec++;
       sum.tv_nsec -= NANOSECONDS; 
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
            diff.tv_nsec = NANOSECONDS + t1->tv_nsec - t2->tv_nsec;
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
