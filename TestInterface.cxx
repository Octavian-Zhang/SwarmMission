// Catch2 Test
#include <catch2/catch.hpp>
#include <unistd.h>
#include <iostream>
#include <future>

#include "SimulinkIPC.h"

TEST_CASE("Swarm Mission Control Interface", "[SwarmMission]")
{
    SimulinkIPC myIPC;

    // myIPC.rlseAlgStart();

    auto _ = std::async(std::launch::async, [&]()
                        {while (true)
    {
        FCUCMD FlightCMD;
        myIPC.rcvFlightCMD(FlightCMD);

        // IndividualUAVCmd CmdFB;
        // myIPC.rcvMissionCmdFB(CmdFB);

        TaskStatus TaskState{};
        myIPC.rcvTaskStatus(TaskState);

        RealUAVStateBus FlightState{};
        myIPC.SndFlightState(FlightState);

        VectorSpeed VecSpd{};
        myIPC.sndVecSpd(VecSpd);

        uint8_T FlightMode{};
        myIPC.sndFlightMode(FlightMode);

        RealUAVStateBus NbrState{};
        myIPC.SetNbrUAVState(NbrState);
    } });

    IndividualUAVCmd myCMD{};
    myIPC.sndMissionCmd(myCMD);
    myIPC.rcvMissionCmdFB(myCMD);
}
