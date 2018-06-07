/*!
 * Log file functions 
 */

#include "acs.h"

FILE *fp;

int acs_log_txt( char *pfx, char *fac, char *txt )
{
   fp = fopen(ACS_LOG_FILE, "a");
   fprintf(fp, "%s_%s_%s\n", pfx, fac, txt ); 
   fclose( fp );

#ifdef DEBUG 
   fprintf(stderr, "%s_%s_%s\n", pfx, fac, txt ); 
#endif

   return ACS_SUCCESS;
}


/*!
 * Ordered shutdown of log file
*/
int acs_log_shutdown( void )
{
   acs_log_txt( ACS_PFX_INF, ACS_FAC_LOG, "Shutting down" );
   p_libsys_shutdown();

   return ACS_SUCCESS;
}
