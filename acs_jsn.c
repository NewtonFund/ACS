#include "acs.h"

// Demand packet is stored locally
static acs_pkt_t      dmd_pkt;                    // Demand packet 
static unsigned char *dmd_jsn_ptr = dmd_pkt.jsn;  // Pointer to start of JSON data 
static unsigned char *dmd_azm_ptr;                // Pointer to Azimuth field within JSON data
static unsigned char *dmd_zd_ptr;                 // Pointer to Zenith Distance field within JSON data
static unsigned char *dmd_rot_ptr;                // Pointer to Rotation field within JSON data
static long int       dmd_len;                    // Total length of a demand packet (packet header + JSON data) 

/*
** Parse a JSON packet
*/
bool acs_jsn_parse( unsigned char *pkt )
{
    unsigned char *chr_ptr;
    unsigned char *quo_ptr; // Pointer to tage name quotation mark 
    unsigned char *sep_ptr; // Pointer to colon separator between tag:value 
    unsigned char *brk_ptr; // Pointer to tag:name terminator in set ", }" 

    unsigned char *nam_ptr; // Tag name 
    unsigned char *val_ptr; // Data value
    unsigned char *cmd_ptr; // Command name
    unsigned char *end_ptr; // End of packet data
    acs_func_t     func;    // Function associated with command

    int            ok = false;
    acs_tag_t     *tag_ptr;

    chr_ptr = pkt;
    if ( !(end_ptr = strchr( pkt, '}' )))  
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Closing } missing from PKT=%s", pkt );
    end_ptr--;

// Search for first brace marking start of JSON and move past it 
    if (( !(chr_ptr = strchr ( chr_ptr, '{'  )))||   // Find first brace { char marking start of JSON data
        ( !(quo_ptr = strchr ( chr_ptr, '\"' )))||   // Find first \" char marking start of tag 
        ( !(sep_ptr = strchr ( chr_ptr, ':'  )))||   // Find separator
        ( !(brk_ptr = strpbrk( chr_ptr, ",}" )))  )  // Find break separating comma or terminating brace  
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "CommandName missing from PKT=%s", pkt );

//  Extract the tag name and check that it is "CommandName"
    if ( !(nam_ptr = strtok( quo_ptr, " \"" )))
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "No tag found in PKT=%s", pkt);
    if (strcmp( nam_ptr, ACS_TAG_COMMANDNAME ))
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Out-of-order TAG=%s. First tag must be CMD=%s", nam_ptr, ACS_TAG_COMMANDNAME);

//  Extract the associated value
    if ( !(cmd_ptr = strtok( ++sep_ptr, " \"}," )))
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "No command tag found in PKT=%s. No action", pkt);
    acs_debug( DBG2, "RXCMD:%s=%s", nam_ptr, cmd_ptr );

    if ( !( func = utl_cmd2func( cmd_ptr )))
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Unrecognised command=%s. No action", cmd_ptr );

//  Read any following tag name:value pairs
    while( brk_ptr < end_ptr )
    {
        chr_ptr = ++brk_ptr;
        if  (( !(quo_ptr = strchr ( chr_ptr, '\"' )))||    // Find first \" char marking start of tag 
             ( !(sep_ptr = strchr ( chr_ptr, ':'  )))||    // Find separator
             ( !(brk_ptr = strpbrk( chr_ptr, ",}" )))  )   // Find break separating comma or terminating brace  
            return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Command not found in JSON packet=%s", pkt );

        if (!(nam_ptr = strtok(   quo_ptr, ", \"" )))      // If no tag then skip 
            break;
        if (!(val_ptr = strtok( ++sep_ptr, "}, \"")))      // If no value then skip 
            break;
        acs_debug( DBG2, "  VAL:%s=%s", nam_ptr, val_ptr );// Useful to see what we got
        
        // Search for the tag name in the look-up table 
        if ( !(tag_ptr = bsearch( &nam_ptr, acs_tags, acs_tags_num, acs_tags_siz, acs_tags_cmp )))
        { 
            acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Unrecognised TAG=%s. Ignoring", nam_ptr);
            break;
        }

//      How to process the value depends on its type
        switch ( tag_ptr->type )
        {
            case ACS_TYP_INT:
                ok = acs_chk_int( val_ptr, tag_ptr->var, tag_ptr );
                break;

            case ACS_TYP_DBL:
                ok = acs_chk_dbl( val_ptr, tag_ptr->var, tag_ptr );
                break;

            case ACS_TYP_STR:
                ok = acs_chk_str( val_ptr, tag_ptr->var, tag_ptr );
                break;

            case ACS_TYP_ENUM:
                ok = false;
//              Determine which ENUM tag type and pass appropriate LUT 
                if      (!strcmp( nam_ptr, ACS_TAG_TRACKEQUINOX  )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_equinox );
                else if (!strcmp( nam_ptr, ACS_TAG_TRACKROTFRAME )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_rotframe );
                else if (!strcmp( nam_ptr, ACS_TAG_CORRFRAME     )||
                         !strcmp( nam_ptr, ACS_TAG_OFFSETFRAME   )||
                         !strcmp( nam_ptr, ACS_TAG_AGFRAME       )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_frame );
                else if (!strcmp( nam_ptr, ACS_TAG_CORRTYPE      )||
                         !strcmp( nam_ptr, ACS_TAG_OFFSETTYPE    )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_type );
                else if (strcmp( nam_ptr, ACS_TAG_AGTYPE         )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_ag );
                else if (strcmp( nam_ptr, ACS_TAG_LOGACTION      )  )
                    ok = acs_chk_enum( val_ptr, tag_ptr->var, tag_ptr, acs_enum_action );
                else
                    ok = acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Unrecognised TAG=%s. Ignoring", nam_ptr);
                break;

            case ACS_TYP_TIME:
                ok = acs_chk_time( val_ptr, tag_ptr->var, tag_ptr );
                break;

            case ACS_TYP_IP:
                ok = acs_chk_ip( val_ptr, tag_ptr->var, tag_ptr );
                break;

            case ACS_TYP_BOOL:
                ok = acs_chk_bool( val_ptr, tag_ptr->var, tag_ptr );
                break;

            default: // This should never happen :-)
                ok = acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Unrecognised value TYPE=%i. Ignoring", tag_ptr->type);
                break;
        }
    }

//  Invoke the function for this command 
    if ( func )    
        return func();
    else
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_JSN, "Function not found for command");
}


/*
 * Build a JSON packet from a supplied list of tag:values
 */
long int acs_jsn_mkpkt( acs_pkt_t *pkt_ptr, unsigned char ack, int type, jsn_tag_t *tags )
{
    bool empty = true;
    bool busy  = true;
    unsigned char *jsn_ptr = pkt_ptr->jsn; 

    jsn_ptr += sprintf( jsn_ptr, "[{" );
   
    while(busy)
    {
        switch (tags->type) 
        {
            case JSN_TYP_INT:
                jsn_ptr += sprintf( jsn_ptr, tags->fmt, tags->name, *(int *)tags->val);
                empty = false;
                break;

            case JSN_TYP_DBL:
                jsn_ptr += sprintf( jsn_ptr, tags->fmt, tags->name, *(double *)tags->val);
                empty = false;
                break;

            case JSN_TYP_STR:
                jsn_ptr += sprintf( jsn_ptr, tags->fmt, tags->name, (char *)tags->val);
                empty = false;
                break;

            case JSN_TYP_END:
                busy = false;
                break;

            default:
//              ERROR: Return zero length
                return 0;
                break;
        }    
        tags++;
    }

//  If not empty, erase the trailing comma separator 
    if (!empty)
        *(--jsn_ptr) = '\0';

//  Add trailing chars. and return total length
    strcat( jsn_ptr, "}]" );


//  Polupulate header
    pkt_ptr->hdr.start = ACS_PKT_START;     // Set packet start marker
    pkt_ptr->hdr.ID++;                      // Increment message ID
    pkt_ptr->hdr.total = 1;                 // Total fragment count (always 1)
    pkt_ptr->hdr.seqn  = 1;                 // This fragment's sequence No. (always 1)
    pkt_ptr->hdr.len   = strlen( jsn_ptr ); // Length of JSON data
    pkt_ptr->hdr.ack   = ack;               // ACK needed, 0=no 1=yes
    pkt_ptr->hdr.type  = type;              // Command type

//  Return total length including header
    return ( sizeof(acs_hdr_t) +  pkt_ptr->hdr.len );
}


/*
 * Write axis values into demand packet template
 */
long int acs_jsn_wrdmd( acs_pkt_t *pkt_ptr )
{
   long int off1;
   long int off2;
   long int off3;

//  Copy values into demand
    off1 = sprintf( dmd_azm_ptr, JSN_FMT_WRDMD, acs_mem->DmdAzm );
    off2 = sprintf( dmd_zd_ptr,  JSN_FMT_WRDMD, acs_mem->DmdZD  );
    off3 = sprintf( dmd_rot_ptr, JSN_FMT_WRDMD, acs_mem->DmdRot );

//  Overwrite nul terminators
    *(dmd_azm_ptr + off1) = ' ';
    *(dmd_zd_ptr  + off2) = ' ';
    *(dmd_rot_ptr + off3) = ' ';

//  Increment unique i packet ID
    pkt_ptr->hdr.ID++;

    return dmd_len;
}


/*
 * Create a demand packet template
 * Subsequently only the demand values change 
 */
long int acs_jsn_mkdmd( acs_pkt_t **pkt_ptr )
{
// Place holder values to locate fields
#define DMD_AZM "-111.0"
#define DMD_ZD  "-222.0"
#define DMD_ROT "-333.0"

    jsn_tag_t tags[6];

    acs_mem->DmdAzm = atof( DMD_AZM );
    acs_mem->DmdZD  = atof( DMD_ZD  );
    acs_mem->DmdRot = atof( DMD_ROT );

//  Assemble a demand packet
    tags[0].type=JSN_TYP_STR;
    tags[0].name=ACS_TAG_COMMANDNAME;
    tags[0].val =ACS_CMD_AXISDEMAND;
    tags[0].fmt =JSN_FMT_STR;
       
    tags[1].type=JSN_TYP_DBL;
    tags[1].name=ACS_TAG_DEMANDAZM;
    tags[1].val =&acs_mem->DmdAzm;
    tags[1].fmt =JSN_FMT_MKDMD;

    tags[2].type=JSN_TYP_DBL;
    tags[2].name=ACS_TAG_DEMANDZD;
    tags[2].val =&acs_mem->DmdZD;
    tags[2].fmt =JSN_FMT_MKDMD;

    tags[3].type=JSN_TYP_DBL;
    tags[3].name=ACS_TAG_DEMANDROT;
    tags[3].val =&acs_mem->DmdRot;
    tags[3].fmt =JSN_FMT_MKDMD;

    tags[4].type=JSN_TYP_STR;
    tags[4].name=ACS_TAG_INSTNAME;
    tags[4].val =&acs_mem->InstName;
    tags[4].fmt =JSN_FMT_STR;

    tags[5].type=JSN_TYP_END;
    tags[5].name="";
    tags[5].val =NULL;
    tags[5].fmt ="";

//  Create template packet and update locally held length 
    dmd_len = acs_jsn_mkpkt( &dmd_pkt, 0, ACS_PKT_TYPE_CALLBACK, tags );  

//  Locate and save place holder addresses in local pointers
    dmd_azm_ptr = strstr( dmd_jsn_ptr, DMD_AZM );
    dmd_zd_ptr  = strstr( dmd_jsn_ptr, DMD_ZD  );
    dmd_rot_ptr = strstr( dmd_jsn_ptr, DMD_ROT );

//  Return the address of the local demand packet by updating supplied pointer
    *pkt_ptr = &dmd_pkt; 

//  Return length 
    return dmd_len;
}
