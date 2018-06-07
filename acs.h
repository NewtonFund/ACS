/*!
 * Generic ACS header file
 */

#define DEBUG 

#define FALSE 0
#define TRUE !(FALSE)


#include <stdio.h>
#include <plibsys.h>
#include <ptypes.h>
#include <plist.h> 
#include <perror.h>
#include <pmacros.h>
#include <perrortypes.h>

#define ACS_SUCCESS 0 
#define ACS_FAIL    1	

#define ACS_INI_FILE      "acs.ini"
#define ACS_INI_NET       "NETWORK"
#define ACS_INI_NET_IP    "NET_IP"
#define ACS_INI_NET_PORT  "NET_PORT"

#define ACS_LOG_FILE      "acs_errlog.log"


// ACS process states
#define ACS_STATE_START      0
#define ACS_STATE_IDLE       1 
#define ACS_STATE_SLEW       2 
#define ACS_STATE_TRACK      3 
#define ACS_STATE_TRACK_WARN 4 
#define ACS_STATE_ERROR      5 


// ACS Commands   
#define ACS_CMD_TRACKINIT    "TrackInit"    // - Tracking initialise     Inbound
#define ACS_CMD_TRACKCORR    "TrackCorr"    // - Tracking corrections    Inbound
#define ACS_CMD_TRACKOFFSET  "TrackOffset"  // - Tracking offsets        Inbound
#define ACS_CMD_TRACKCONTROL "TrackControl" // - Tracking control        Inbound
#define ACS_CMD_TRACKEND     "TrackEnd"     // - Tracking end            Inbound
#define ACS_CMD_TRACKTORADEC "TrackToRADec" // - Tracking to RA/DEC      Inbound
#define ACS_CMD_TRACKWRITE   "TrackWrite"   // - Write tracking info     Inbound   Engineering
#define ACS_CMD_TRACKREAD    "TrackRead"    // - Read tracking info      Inbound   Engineering
                 
#define ACS_CMD_AGCORR       "AGCorr"      // - Autoguider correction   Inbound
#define ACS_CMD_AGDATUM      "AGDatum"     // - Autoguider datum        Inbound

#define ACS_CMD_METINFO      "MetInfo"     // - Meteorology information Inbound
   
#define ACS_CMD_INSTINFO     "InstInfo"    // - Instrument information  Inbound

#define ACS_CMD_AXISINFO     "AxisInfo"    // - Axis information        Inbound
#define ACS_CMD_AXISDEMAND   "AxisDemand"  // - Axis demand             Outbound

#define ACS_CMD_ACSGETPARAM  "ACSGetParam" // - Get parameter           Inbound
#define ACS_CMD_ACSSETPRARM  "ACSSetParam" // - Set parameter           Inbound
#define ACS_CMD_ACSRECONFIG  "ACSReconfig" // - Re-read configuration   Inbound 
#define ACS_CMD_ACSTRANS     "ACSTrans"    // - State transition        Outbound
#define ACS_CMD_ACSREPLY     "ACSReply"    // - Reply                   Outbound





// Error logging 
// Severerity prefix
#define ACS_PFX_INF      "INF"    // Information
#define ACS_PFX_WRN      "WRN"    // Warning 
#define ACS_PFX_ERR      "ERR"    // Error
#define ACS_PFX_SYS      "SYS"    // System


// Facility name prefix
#define ACS_FAC_CFG      "CFG"
#define ACS_FAC_NET      "NET"
#define ACS_FAC_LOG      "LOG"





// Global Function Prototypes
int acs_ini_read( void );
int acs_log_txt( char *pfx, char *fac, char *txt );
int acs_log_shutdown( void );
