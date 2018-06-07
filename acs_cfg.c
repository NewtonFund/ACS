/*!
 * Initialisation functions 
 */

#include "acs.h"
#include "acs_cfg.h"


/*!
 * Read configuration .ini file
 */
int acs_ini_read( void )
{
   int i;

   PIniFile *file;
   PError   *error;

   file = p_ini_file_new( ACS_INI_FILE );

   if ( !p_ini_file_parse( file, &error ) ) {
       acs_log_txt( ACS_PFX_ERR, ACS_FAC_CFG, "Failed to read .ini file" );
   } else {
       for( i = acs_config_siz; --i ; ) {
           switch (acs_config[i].type) {
               case CFG_TYPE_INT:
                   *(int *)acs_config[i].ptr = p_ini_file_parameter_int( file, acs_config[i].sect, acs_config[i].key, 0 );
#ifdef DEBUG
    printf( "%s=%i\n", acs_config[i].key, *(int *)acs_config[i].ptr );
#endif
                   break;

               case CFG_TYPE_DOUBLE:
                   *(double *)acs_config[i].ptr = p_ini_file_parameter_double( file, acs_config[i].sect, acs_config[i].key, 0.0 );
#ifdef DEBUG
    printf( "%s=%f\n", acs_config[i].key, *(double *)acs_config[i].ptr );
#endif
                   break;

               case CFG_TYPE_BOOLEAN:
                   *(unsigned char *)acs_config[i].ptr = p_ini_file_parameter_boolean( file, acs_config[i].sect, acs_config[i].key, FALSE );
                   break;

               case CFG_TYPE_STRING:
                   strcpy( (char *)acs_config[i].ptr, (char *)p_ini_file_parameter_string( file, acs_config[i].sect, acs_config[i].key, ""));
                   break;

               default:
                   break;
           }
       }   
   }

   return ACS_SUCCESS;
}
