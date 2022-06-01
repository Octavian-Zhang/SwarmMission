#ifndef SWARM_BASIC_DATA_H
#define SWARM_BASIC_DATA_H

#include "DatalinkInterface.h"

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

struct StateFCU
{
    RealUAVStateBus RealUAVState;
    VectorSpeed VecSpd;
};

struct ExtU_codegenReal2Mission_T {
    uint8_T FlightMode;            // '<Root>/FlightMode'
    StateFCU FlightState;          // '<Root>/FlightState'
};

struct FCUCMD
{
    real_T Latitude_deg;
    real_T Longitude_deg;
    real_T Height_meter;
    real_T RefHdg_deg;
    real_T RefAirSpd_mps;
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

struct ExtY_codegenReal2Mission_T {
    FCUCMD FlightCMD;              // '<Root>/FlightCMD'
    TaskStatus MissionFB;          // '<Root>/MissionFB'
};

#endif
