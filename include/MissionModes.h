//
// File: MissionModes.h
//
// Code generated for Simulink model 'codegenReal2Mission'.
//
// Model version                  : 5.25
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Mon Nov 21 19:28:33 2022
//

#ifndef RTW_HEADER_MissionModes_h_
#define RTW_HEADER_MissionModes_h_
#include "rtwtypes.h"

// Enumeration class that captures all mission modes
enum class MissionModes
    : int32_T {
    CircNav = 1,
    HorzScanNav = 2,
    RunWayNav = 3,
    DetailedInsp = 128,
    AvoidDanger = 129,
    FlightMissionRH = 130,
    pAttack = 133,
    CircDispNav = 11,
    CustomFrmnNav = 22,
    FrmnRunWay = 33,
    ProtLine = 44,
    FrmnWayPoint = 55,
    WaitToStart = 255,
    None = 0,                          // Default value
    TuneADRC = 252,
    ShiftEarthENU = 254,
    ShiftBodyXYZ = 253
};

#endif                                 // RTW_HEADER_MissionModes_h_

//
// File trailer for generated code.
//
// [EOF]
//
