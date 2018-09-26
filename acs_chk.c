/*
 * Functions to check tag values and validate actions
 */
#include "acs.h"

/*
 * Check integer is within range 
 * Returns: true | false
 */
int acs_chk_bool( unsigned char *str, bool *boo, acs_tag_t *tag )
{
    unsigned char *ptr; 

//  Convert input string to lower case before comparison
    for( ptr = str; *ptr; tolower(*ptr++))
        ; 

//  Check against true/false text
    if ( !strcmp( str, "true" ))
    {
        if ( boo )
            *boo = true; 
        return true;
    }
    else if (!strcmp( str, "false" ))
    {
        if ( boo )
            *boo = false; 
        return false;
    } 
    else
    { 
//      Indicate error 
        return acs_log_msg( ACS_ERROR, ACS_PFX_WRN, ACS_FAC_CHK, "Check failed for TAG=%s:BOOL=%s", tag->name, str );
    }
}

/*
 * Check integer is within range 
 * Returns: true | false
 */
int acs_chk_int( unsigned char *str, int *var, acs_tag_t *tag  )
{
    int           i;           // Local integer value    
    unsigned char chr;         // Picket to catch extra trailing characters

//  Attempt to convert to integer. There should only be 1 conversion 
    if ( 1 != sscanf( str, "%i%c", &i, &chr )) 
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Check failed for TAG=%s:INT=%s", tag->name, str );

//  Check that converted value is within range
    if ( (i > (int)tag->max) || (i < (int)tag->min) )
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Out-of-range TAG=%s:INT=%s:MAX=%i:MIN=%i", tag->name, str, (int)tag->max, (int)tag->min);

//  If a target variable address provided then update it 
    if ( var )
       *var = i;
      
    return true; 
}


/*
 * Check double is within range 
 * Parameters: *str - pointer to tag string to be read 
 *             *var - pointer to variable to be written. If NULL then limit check is performed with no update
 *             *tag = pointer to tag table entry  
 * Returns: true || false
 */
int acs_chk_dbl( unsigned char *str, double *var, acs_tag_t *tag )
{
    double        dbl;         // Local float value    
    unsigned char chr;         // Picket to catch extra trailing characters

//  Attempt to convert to float. There should only be 1 conversion 
    if ( 1 != sscanf( str, "%lf%c", &dbl, &chr )) 
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Check failed for TAG=%s:DBL=%s", tag->name, str );

//  Check read value is within range
    if ( (dbl > tag->max) || (dbl < tag->min) )
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Out-of-range TAG=%s:DBL=%s:MAX=%e:MIN=%e", tag->name, str, tag->max, tag->min);

//  If a target variable address provided then update it 
    if ( var )
       *var = dbl;
      
    return true; 
}


/*
 * Check string
 * Returns:  0  String is empty  (Invalid)
 *          >0  Length of string (Valid) 
 */
int acs_chk_str( unsigned char *str, char *var, acs_tag_t *tag )
{
    if ( var )
        strncpy( var, str, STR_MAX ); 

    return ( strlen( str ) );
}


/*
 * Convert string to an index value used as a pseudo enumeration
 * This is dumb sequential search only suitable for small tables
 *
 * Returns: true is match found else false 
 *          Updates destination variable *var if not NULL
 */
int acs_chk_enum( unsigned char *str, int *var, acs_tag_t *tag, acs_enum_t *tbl )
{
//  Search through provided table. The table end is marked with a nul string 
    while( tbl->str )
        if ( !strcmp( tbl->str, str ))
            break;
        else
           tbl++;

//  If no matching string found return failure 
    if (!tbl->str)
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Enum lookup of failed for TAG=%s", str);

//  Write look-up value 
    if ( var )
        *var = tbl->val;

    return true;
}


/*
 * Check time string is valid 
 * Returns the time structure or a pointer to NULL if invalid
 */
int acs_chk_time( unsigned char *str, acs_time_t *var, acs_tag_t *tag )
{
    struct tm t; // Date and time to 1 sec resolution 
    double frac; // Fractional part of sec
    char   chr;  // Used to check for extraneous trailing chars

//  Parse the time string which must be UTC in format YYYY-MM-DDTHH:MM:SS.sss
    if ( 7 != sscanf( str, "%d-%d-%dT%d:%d:%d%lf%c",
                      &t.tm_year, &t.tm_mon, &t.tm_mday, 
                      &t.tm_hour, &t.tm_min, &t.tm_sec, &frac, &chr ) )
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Failed to parse TIME=%s", str);

//  Convert to IAU format pair of doubles
    if ( iauDtf2d( "UTC", t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec+frac, &var->utc[0], &var->utc[1] )) 
        acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "iauDtf2D() failed to convert time");

//  Adjust to make values C-like and convert to
    t.tm_mon--;
    t.tm_year -= 1900;
    mktime( &t );
}

/*
 * Check network is valid 
 */
int acs_chk_ip( unsigned char *str, char *var, acs_tag_t *tag )
{
    if ( var )
        strncpy( var, str, STR_MAX );

    return strlen( str );;
}

