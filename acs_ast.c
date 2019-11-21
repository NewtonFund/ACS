#include "acs.h"

int acs_ast_calc(struct timespec *ts )
{

    double utc1, utc2; // UTC 
    struct tm t;
    double RA;     // Raw requested ICRS RA 
    double Dec;    // Raw requested ICRS Dec

    struct DeltaTime_ts; // Time Delta as a time structure

//  Local returned values are in radians
    static double DmdAzm;  // [radians] Azimult angle
    static double DmdZD;   // [radians] Zenith distance
    static double DmdRot;  // [radians] Rotator angle
    static double DmdDec;  // [radians] Declination
    static double DmdRA;   // [radians] RA
    static double DmdEO;   // Equation of origin (ERA -GST)
    static double InstRot;  
    static double AGRot;  

/*
  Get time now
  clock_gettime( CLOCK_REALTIME, &ts );
*/

    RA  = acs_mem->ReqRadRA;
    Dec = acs_mem->ReqRadDec;

//  Add CELESTial corrections to RA and DEC 
    if (( acs_mem->EnableCorr                    )&& 
        ( acs_mem->CorrFrame == ACS_FRAME_CELEST )  ) 
    { 
        acs_debug( DBG3, "CorrCelest %f %f", acs_mem->CorrX, acs_mem->CorrY );
        RA  += acs_mem->CorrX;
        Dec += acs_mem->CorrY;
    }

    if (( acs_mem->EnableOffset                    )&& 
        ( acs_mem->OffsetFrame == ACS_FRAME_CELEST )  ) 
    { 
        acs_debug( DBG3, "OffsetCelest %f %f", acs_mem->OffsetX, acs_mem->OffsetY );
        RA  += acs_mem->OffsetX;
        Dec += acs_mem->OffsetY;
    }
   

//  Get time now and add any requested time offset 
//  *ts = utl_ts_add( ts, utl_dbl2ts( acs_mem->DeltaTime ) ); 
    t  = *gmtime( &ts->tv_sec);
    if ( iauDtf2d ( "UTC", 1900+t.tm_year, 1+t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec+ts->tv_nsec/NANOSECOND, &utc1, &utc2 ) )
    {
       return ACS_FAILURE;
    }

//    if ( iauUtctai( utc1, utc2, &tai1, &tai2 ) ) return -1;
//    if ( iauTaitt ( tai1, tai2, &tt1,  &tt2  ) ) return -1;

    if ( iauAtco13( RA, Dec,
                   (acs_mem->EnablePMRA  ? acs_mem->TrackPMRA  : 0.0), // Include proper motions ?
                   (acs_mem->EnablePMDec ? acs_mem->TrackPMDec : 0.0), 
                    acs_mem->TrackParallax, 
                    acs_mem->TrackRadVel,
                    utc1, utc2, acs_mem->DUT1,
                    acs_mem->Lon, acs_mem->Lat, acs_mem->Alt,
                    acs_mem->PolarX, acs_mem->PolarY,
//                  Setting pressure to 0.0 suppresses refraction correction
                   (acs_mem->EnableMet ? acs_mem->Press : 0.0), acs_mem->Temp, acs_mem->RH, acs_mem->InstWavelength,
                   &DmdAzm, &DmdZD, &DmdRot, &DmdDec, &DmdRA, &DmdEO ) )
    { 
        return ACS_FAILURE;
    }
    else
    { 
         InstRot = acs_mem->InstRotAngle + DmdRot;  
         AGRot   = acs_mem->AGRotAngle   + DmdRot;  

         DmdAzm *= DR2D;
         DmdZD  *= DR2D;

// Work in degrees from here ---------------------------------------------------------------------------

//      Add mount and instrument corrections  
        if ( acs_mem->EnableCorr ) 
        { 
            if ( acs_mem->CorrFrame == ACS_FRAME_MOUNT ) 
            { 
                acs_debug( DBG3, "CorrMount X=%f Y=%f", acs_mem->CorrX, acs_mem->CorrY );
                DmdAzm += acs_mem->CorrX; 
                DmdZD  += acs_mem->CorrY; 
                DmdRot += acs_mem->CorrRot; 
            }
            else if ( acs_mem->CorrFrame == ACS_FRAME_INST ) 
            { 
                acs_debug( DBG3, "CorrInst X=%f Y=%f", acs_mem->CorrX, acs_mem->CorrY );
//              If instrument rotates with telescope then add current angle
                if ( acs_mem->InstRotates )
                    InstRot = acs_mem->InstRotAngle + DmdRot;  
                else
                    InstRot = acs_mem->InstRotAngle;  

//              LT like telescope geometry is assumed. 0 degree = North and telescope tilts north               
//              Check if instrument angle is West or East of 0(=North) degrees               
                if ( InstRot < 0 )
                {
                    DmdAzm += sin(InstRot) * acs_mem->CorrY - cos(InstRot) * acs_mem->CorrX; 
                    DmdZD  += sin(InstRot) * acs_mem->CorrX - cos(InstRot) * acs_mem->CorrY; 
                }
                else 
                {
                    DmdAzm += sin(InstRot) * acs_mem->CorrY + cos(InstRot) * acs_mem->CorrX; 
                    DmdZD  += sin(InstRot) * acs_mem->CorrY + cos(InstRot) * acs_mem->CorrX; 
                }
            }
        }

//      Add mount and instrument offset here 
        if ( acs_mem->EnableAG ) 
        { 
            if ( acs_mem->AGFrame == ACS_FRAME_MOUNT ) 
            { 
                acs_debug( DBG3, "AGMount %f %f", acs_mem->AGX, acs_mem->AGY );
                DmdAzm += acs_mem->AGX; 
                DmdZD  += acs_mem->AGY; 
                DmdRot += acs_mem->AGRotAngle; 
            }
            else if ( acs_mem->AGFrame == ACS_FRAME_INST ) 
            { 
                acs_debug( DBG3, "AGInst %f %f", acs_mem->AGX, acs_mem->AGY );
//              If instrument rotates with telescope then add current angle
                if ( acs_mem->AGRotates )
                    InstRot = acs_mem->AGRotAngle + DmdRot;  
                else
                    InstRot = acs_mem->AGRotAngle;  

//              LT like telescope geometry is assumed. 0 degree = North and telescope tilts north               
//              Check if instrument angle is West or East of 0(=North) degrees               
                if ( AGRot < 0 )
                {
                    DmdAzm += sin(AGRot) * acs_mem->AGY - cos(AGRot) * acs_mem->AGX; 
                    DmdZD  += sin(AGRot) * acs_mem->AGX - cos(AGRot) * acs_mem->AGY; 
                }
                else 
                {
                    DmdAzm += sin(AGRot) * acs_mem->AGY + cos(AGRot) * acs_mem->AGX; 
                    DmdZD  += sin(AGRot) * acs_mem->AGY + cos(AGRot) * acs_mem->AGX; 
                }
            }
        }


//      Update all enabled axis demands otherwise they remain fixed
        if ( acs_mem->EnableAzm )
             acs_mem->DmdAzm = DmdAzm;
        if ( acs_mem->EnableZD )
             acs_mem->DmdZD  = DmdZD ;
        if ( acs_mem->EnableRot )
             acs_mem->DmdRot = DmdRot * DR2D;

        acs_debug( DBG3, "Demand: ZD=%.15f\t AZ=%.15f", DmdZD, DmdAzm );
    }

    return ACS_SUCCESS;
}
