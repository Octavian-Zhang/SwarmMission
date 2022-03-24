#ifndef SWARM_BASIC_DATA_H
#define SWARM_BASIC_DATA_H

#include "DatalinkInterface.h"

struct RealUAVStateBus
{
    real_T Latitude_deg;
    real_T Longitude_deg;
    real_T Height_meter;
    real_T AirSpeed_mps;
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
    RealUAVStateBus RealUAVState;   // @key
    real_T GndSpd_mps;              // @key, Ground Speed
    real_T Altitude;
    VectorSpeed VecSpd;
};

struct ExtU_codegenReal2Mission_T
{
    uint8_T FlightMode;                 // '<Root>/FlightMode'
    RealUAVStateBus OtherUAVstate[128]; // '<Root>/OtherUAVstate'
    StateFCU StateFCU_b;                // '<Root>/StateFCU'
};

struct FCUCMD
{
    real_T Latitude_deg;
    real_T Longitude_deg;
    real_T Height_meter;
    real_T RefAirSpd_mps;
};

struct TaskStatus
{
    boolean_T EngagedFlag;
    int32_T FlightStatus;
    int32_T ImmedStatus;
    int32_T SequenceID;
    MissionModes MissionMode;
    int32_T numUAV;
    int32_T FormationPos;
};

struct ExtY_codegenReal2Mission_T
{
    FCUCMD FCUCMD_i;      // '<Root>/FCUCMD'
    TaskStatus MissionFB; // '<Root>/MissionFB'
};

#endif
