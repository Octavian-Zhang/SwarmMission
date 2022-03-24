//
// File: DatalinkInterface.h
//
// Code generated for Simulink model 'codegenReal2Mission'.
//
// Model version                  : 3.91
// Simulink Coder version         : 9.6 (R2021b) 14-May-2021
// C/C++ source code generated on : Wed Feb 23 00:38:39 2022
//

#ifndef RTW_HEADER_DatalinkInterface_h_
#define RTW_HEADER_DatalinkInterface_h_
#include "MissionModes.h"
#include "rtwtypes.h"

struct Location
{
    real_T Lat;
    real_T Lon;
    real_T Alt;
    real_T degHDG;
};

struct Parameters
{
    real32_T Param1;
    real32_T Param2;
    real32_T Param3;
    real32_T Param4;
    real32_T Param5;
    real32_T Param6;
    real32_T Param7;
};

struct Time
{
    int32_T year;
    int32_T month;
    int32_T day;
    int32_T hour;
    int32_T minute;
    int32_T second;
    int32_T millisecond;
};

struct IndividualUAVCmd
{
    int32_T SequenceID;
    MissionModes MissionMode;
    Location MissionLocation;
    Parameters params;
    Location StartPosition;
    int32_T numUAV;
    int32_T FormationPos;
    Time StartTime;
};

#endif                                 // RTW_HEADER_DatalinkInterface_h_

//
// File trailer for generated code.
//
// [EOF]
//
