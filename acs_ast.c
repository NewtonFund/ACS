#include "acs.h"
#include "acs_dat.h"

#define NANO 1000000000.0

int acs_ast_calc(void)
{
    double utc1, utc2; // UTC 
//  double tai1, tai2; // TAI atomic time 
//  double tt1,  tt2;  // TT  terrestrial time
    struct timespec ts;
    struct tm t;

    double ra;     // Raw requested ICRS RA 
    double dec;    // Raw requested ICRS Dec

    double az_dmd; // Azimuth [radians, N=0, E=+90, S=+180, W=+270]
    double zd_dmd; // Zenith distance [radians]
    double ha_dmd; // Hour angle [radians] 
    double dec_dmd;// DEC [radians]
    double ra_dmd; // RA  [radians] 
    double eo_dmd; // Equation of origins (ERA - GST) 

//  Convert ICRS decimal RA and DEC to radians 
    ra  = acs_mem->TrackRA  * D2PI / 24.0;
    dec = acs_mem->TrackDec * DAS2R;

//  Get time now
    clock_gettime( CLOCK_REALTIME, &ts );
    t = *gmtime( &ts.tv_sec);

    if ( iauDtf2d ( "UTC", 1900+t.tm_year, 1+t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec+ts.tv_nsec/NANO, &utc1, &utc2 ) )
    {
       return ACS_FAILURE;
    }
//    if ( iauUtctai( utc1, utc2, &tai1, &tai2 ) ) return -1;
//    if ( iauTaitt ( tai1, tai2, &tt1,  &tt2  ) ) return -1;

    if ( iauAtco13( ra, dec,
                   (acs_mem->EnablePMRA  ? acs_mem->TrackPMRA  : 0.0),
                   (acs_mem->EnablePMDec ? acs_mem->TrackPMDec : 0.0), 
                    acs_mem->TrackParallax, 
                    acs_mem->TrackRadVel,
                    utc1, utc2, acs_mem->DUT1,
                    acs_mem->Lon, acs_mem->Lat, acs_mem->Alt,
                    acs_mem->PolarX, acs_mem->PolarY,
                    acs_mem->Press,  acs_mem->Temp, acs_mem->RH, acs_mem->InstWavelength,
                    &az_dmd, &zd_dmd, &ha_dmd, &dec_dmd, &ra_dmd, &eo_dmd ) )
    { 
        return ACS_FAILURE;
    }
    else
    { 
        acs_debug( DBG3, "Demand: ZD=%.15f\t AZ=%.15f", zd_dmd * DR2D, az_dmd * DR2D );
    }

    return ACS_SUCCESS;
}
                            
