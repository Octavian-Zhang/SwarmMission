//
// File: DatalinkInterface.h
//
// Code generated for Simulink model 'codegenReal2Mission'.
//
// Model version                  : 5.25
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Mon Nov 21 19:28:33 2022
//

#ifndef RTW_HEADER_DatalinkInterface_h_
#define RTW_HEADER_DatalinkInterface_h_
#include "rtwtypes.h"
#include "MissionModes.h"

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

struct RealUAVStateBus
{
    int32_T UAV_ID;
    real_T Latitude_deg;
    real_T Longitude_deg;
    real_T Height_meter;
    real_T Altitude_meter;
    real_T AirSpeed_mps;
    real_T GndSpd_mps;
    real_T HeadingAngle_deg;
    real_T FlightPathAngle_deg;
    real_T RollAngle_deg;
};

struct VectorSpeed
{
    real_T eastSpeed;
    real_T northSpeed;
    real_T skySpeed;
};

struct FCUCMD
{
    real_T RefLatitude_deg;
    real_T RefLongitude_deg;
    real_T RefHeight_meter;
    real_T RefHdg_deg;
    real_T RefAirSpd_mps;
    real_T RefGndSpd_mps;
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

struct TaskStatus
{
    uint8_T EngagedFlag;
    int32_T FlightStatus;
    int32_T ImmedStatus;
    int32_T SequenceID;
    MissionModes MissionMode;
    int32_T numUAV;
    int32_T FormationPos;
};

#endif                                 // RTW_HEADER_DatalinkInterface_h_

//
// File trailer for generated code.
//
// [EOF]
//
