/*
 * Functions to process inbound and outbound commands
 * NOTE: THESE ARE PLACEHOLDERS FOR SEPTEMBER DEMO
 *       ONLY DATA IS COPIED TO SHARED MEMEORY WITHOUT STATE VALIDATION
 */
#include "acs.h"

// Command name       Function             Type    
int acs_cmd_TrackInit  (void)
{
    acs_mem->TrackEquinox  = acs_TrackEquinox; 
    acs_mem->TrackEpoch    = acs_TrackEpoch; 
    acs_mem->TrackRA       = acs_TrackRA; 
    acs_mem->TrackDec      = acs_TrackDec; 
    acs_mem->TrackPMRA     = acs_TrackPMRA; 
    acs_mem->TrackPMDec    = acs_TrackPMDec; 
    acs_mem->TrackParallax = acs_TrackParallax; 
    acs_mem->TrackRadVel   = acs_TrackRadVel; 
    acs_mem->TrackRotFrame = acs_TrackRotFrame; 
    acs_mem->TrackRotAngle = acs_TrackRotAngle; 
    acs_mem->TrackFreq     = acs_TrackFreq; 
    acs_mem->TrackStart    = acs_TrackStart; 
    acs_mem->TrackDuration = acs_TrackDuration; 

    acs_mem->EnableCorr    = acs_EnableCorr; 
    acs_mem->EnableOffset  = acs_EnableOffset; 
    acs_mem->EnableAG      = acs_EnableAG; 

//  Convert input RA & DEC to radians
    acs_mem->ReqRadRA      = acs_TrackRA  * D2PI / 24.0;     
    acs_mem->ReqRadDec     = acs_TrackDec * DAS2R;     

    acs_mem->Ready = true;    

    return true;
}

int acs_cmd_TrackCorr  (void)
{
    if      ( ( acs_CorrFrame == ACS_FRAME_MOUNT )&&
              ( acs_CorrType  == ACS_TYPE_FIXED  )  )
    {
        acs_mem->CorrX    = acs_CorrX   * MAS2DEG;
        acs_mem->CorrY    = acs_CorrY   * MAS2DEG;
        acs_mem->CorrRot  = acs_CorrRot * MAS2DEG;
    }
    else if ( ( acs_CorrFrame == ACS_FRAME_MOUNT )&&
              ( acs_CorrType  == ACS_TYPE_CUMUL  )  )
    {
        acs_mem->CorrX   += acs_CorrX   * MAS2DEG;
        acs_mem->CorrY   += acs_CorrY   * MAS2DEG;
        acs_mem->CorrRot      += acs_CorrRot * MAS2DEG;

//      Reset globals if cumulative to avoid incrementing each time  
        acs_CorrX = acs_CorrY = acs_CorrRot = 0.0;
    }
    else if ( ( acs_CorrFrame == ACS_FRAME_CELEST)&&
              ( acs_CorrType  == ACS_TYPE_FIXED  )  )
    {
        acs_mem->CorrX   = acs_CorrX * MAS2RAD;
        acs_mem->CorrY   = acs_CorrY * MAS2RAD;
    }
    else if ( ( acs_CorrFrame == ACS_FRAME_CELEST)&&
              ( acs_CorrType  == ACS_TYPE_CUMUL  )  )
    {
        acs_mem->CorrX  += acs_CorrX  * MAS2RAD;
        acs_mem->CorrY  += acs_CorrY  * MAS2RAD;

//      Reset globals if cumulative to avoid incrementing each time  
        acs_CorrX = acs_CorrY = 0.0;
    }
    else if ( ( acs_CorrFrame == ACS_FRAME_INST )&&
              ( acs_CorrType  == ACS_TYPE_FIXED )  )
    {
        acs_mem->CorrX    = acs_CorrX  * MAS2DEG;
        acs_mem->CorrY    = acs_CorrY  * MAS2DEG;
    }
    else if ( ( acs_CorrFrame == ACS_FRAME_INST )&&
              ( acs_CorrType  == ACS_TYPE_CUMUL  )  )
    {
        acs_mem->CorrX   += acs_CorrX  * MAS2DEG;
        acs_mem->CorrY   += acs_CorrY  * MAS2DEG;

//      Reset globals if cumulative to avoid incrementing each time  
        acs_CorrX = acs_CorrY = 0.0;
    }
    else 
    {
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Unrecognised correction" );
    }

    return true;
}

int acs_cmd_TrackOffset(void)
{
    acs_mem->OffsetFrame = acs_OffsetFrame;
    acs_mem->OffsetType  = acs_OffsetType;

    if      ( ( acs_OffsetFrame == ACS_FRAME_MOUNT )&&
              ( acs_OffsetType  == ACS_TYPE_FIXED  )  )
    {
        acs_mem->OffsetX    = acs_OffsetX   * MAS2DEG; 
        acs_mem->OffsetY    = acs_OffsetY   * MAS2DEG; 
        acs_mem->OffsetRot  = acs_OffsetRot * MAS2DEG; 
    }
    else if ( ( acs_OffsetFrame == ACS_FRAME_MOUNT )&&
              ( acs_OffsetType  == ACS_TYPE_CUMUL  )  )
    {
        acs_mem->OffsetX   += acs_OffsetX   * MAS2DEG; 
        acs_mem->OffsetY   += acs_OffsetY   * MAS2DEG; 
        acs_mem->OffsetRot += acs_OffsetRot * MAS2DEG; 

//      Reset globals if cumulative to avoid incrementing each time  
        acs_OffsetX = acs_OffsetY = acs_OffsetRot = 0.0;
    }
    else if ( ( acs_OffsetFrame == ACS_FRAME_CELEST)&&
              ( acs_OffsetType  == ACS_TYPE_FIXED  )  )
    {
        acs_mem->OffsetX   = acs_OffsetX * MAS2RAD; 
        acs_mem->OffsetY   = acs_OffsetY * MAS2RAD; 
    }
    else if ( ( acs_OffsetFrame == ACS_FRAME_CELEST)&&
              ( acs_OffsetType  == ACS_TYPE_CUMUL  )  )
    {
        acs_mem->OffsetX  += acs_OffsetX  * MAS2RAD; 
        acs_mem->OffsetY  += acs_OffsetY  * MAS2RAD; 

//      Reset globals if cumulative to avoid incrementing each time  
        acs_OffsetX = acs_OffsetY = 0.0;
    }
    else if ( ( acs_OffsetFrame == ACS_FRAME_INST )&&
              ( acs_OffsetType  == ACS_TYPE_FIXED )  )
    {
        acs_mem->OffsetX     = acs_OffsetX  * MAS2DEG; 
        acs_mem->OffsetY     = acs_OffsetY  * MAS2DEG; 
    }
    else if ( ( acs_OffsetFrame == ACS_FRAME_INST )&&
              ( acs_OffsetType  == ACS_TYPE_CUMUL )  )
    {
        acs_mem->OffsetX    += acs_OffsetX  * MAS2DEG; 
        acs_mem->OffsetY    += acs_OffsetY  * MAS2DEG; 

//      Reset globals if cumulative to avoid incrementing each time  
        acs_OffsetX = acs_OffsetY = 0.0;
    }
    else 
    {
        return acs_log_msg( false, ACS_PFX_WRN, ACS_FAC_CHK, "Unrecognised Offset: Type=%i Frame=%i", acs_OffsetType, acs_OffsetFrame );
    }

    return true;
}

int acs_cmd_TrackEnable(void)
{
    acs_mem->EnableAzm    = acs_EnableAzm; 
    acs_mem->EnableZD     = acs_EnableZD; 
    acs_mem->EnableRot    = acs_EnableRot; 

    acs_mem->EnableAG     = acs_EnableAG; 
    acs_mem->EnableMet    = acs_EnableMet; 
    acs_mem->EnableCorr   = acs_EnableCorr; 
    acs_mem->EnableOffset = acs_EnableOffset; 
    acs_mem->EnablePMRA   = acs_EnablePMRA; 
    acs_mem->EnablePMDec  = acs_EnablePMDec; 
    return true;
}

int acs_cmd_TrackEnd   (void)
{
    acs_mem->Ready = false;    
    return true;
}

int acs_cmd_TrackWrite (void) 
{
    return true;
}

int acs_cmd_TrackRead  (void) 
{
    return true;
}

int acs_cmd_TrackRADec (void)
{
    return true;
}

int acs_cmd_AGCorr     (void)
{
    acs_mem->AGFrame      = acs_AGFrame;
    acs_mem->AGX          = acs_AGX;
    acs_mem->AGY          = acs_AGY;
    acs_mem->AGType       = acs_AGType;
    acs_mem->AGRotAngle   = acs_AGRotAngle;
    acs_mem->AGRotates    = acs_AGRotates;
    acs_mem->AGWavelength = acs_AGWavelength;
    return true;
}

int acs_cmd_AGDatum    (void)
{
    return true;
}

int acs_cmd_MetInfo    (void)
{
    acs_mem->MetTemp  = acs_MetTemp;
    acs_mem->MetPress = acs_MetPress;
    acs_mem->MetRH    = acs_MetRH;
    return true;
}

int acs_cmd_InstInfo   (void)
{
    strncpy( acs_mem->InstName, acs_InstName, STR_MAX );
    acs_mem->InstRotAngle   = acs_InstRotAngle;
    acs_mem->InstRotates    = acs_InstRotates;
    acs_mem->InstWavelength = acs_InstWavelength;
    return true;
}

int acs_cmd_AxisInfo   (void)
{
    return true;
}

int acs_cmd_AxisDemand (void)
{
    return true;
}

int acs_cmd_ACSGetParam(void)
{
    return true;
}

int acs_cmd_ACSSetParam(void)
{
    return true;
}

int acs_cmd_ACSReconfig(void)
{
    return true;
}

int acs_cmd_ACSChange  (void)
{
//  No parameters
    return true;
}

int acs_cmd_ACSReply   (void)  
{
    return true;
}
