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

//  
    acs_mem->ReqRadRA      = acs_TrackRA  * D2PI / 24.0;     
    acs_mem->ReqRadDec     = acs_TrackDec * DAS2R;     

    return true;
}

int acs_cmd_TrackCorr  (void)
{
    acs_mem->CorrType     = acs_CorrType; 
    acs_mem->CorrFrame    = acs_CorrFrame; 
    acs_mem->CorrX        = acs_CorrX; 
    acs_mem->CorrY        = acs_CorrY; 
    acs_mem->CorrRotAngle = acs_CorrRotAngle; 
    return true;
}

int acs_cmd_TrackOffset(void)
{
    acs_mem->OffsetType     = acs_OffsetType; 
    acs_mem->OffsetFrame    = acs_OffsetFrame; 
    acs_mem->OffsetX        = acs_OffsetX; 
    acs_mem->OffsetY        = acs_OffsetY; 
    acs_mem->OffsetRotAngle = acs_OffsetRotAngle; 
    return true;
    return true;
}

int acs_cmd_TrackEnable(void)
{
    acs_mem->EnableAG     = acs_EnableAG; 
    acs_mem->EnableMet    = acs_EnableMet; 
    acs_mem->EnableCorr   = acs_EnableCorr; 
    acs_mem->EnablePMRA   = acs_EnablePMRA; 
    acs_mem->EnablePMDec  = acs_EnablePMDec; 
    acs_mem->EnableZD     = acs_EnableZD; 
    acs_mem->EnableAzm    = acs_EnableAzm; 
    acs_mem->EnableRot    = acs_EnableRot; 
    return true;
}

int acs_cmd_TrackEnd   (void)
{
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
