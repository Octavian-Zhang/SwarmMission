// Catch2 Test
#include <catch2/catch_test_macros.hpp>
#include <unistd.h>

#include "SwarmControlInterface.h"

TEST_CASE("Swarm Mission Control Interface", "[SwarmMission]")
{
    SimulinkIPC myIPC;
    SwarmControlInterface myInterface;
    myIPC.setSwarmInterface(&myInterface);
    // myIPC.runThreads();

    // myInterface.calibrateTime();

    // FCUCMD myFlightCMD;
    // myFlightCMD = myInterface.getFlightCMD();
    // REQUIRE(myFlightCMD.Height_meter >= 80);
    // REQUIRE(myFlightCMD.RefAirSpd_mps >= 29);

    RealUAVStateBus myNbrState{};
    myNbrState.UAV_ID = 1;
    while (true)
    {
        /* code */
        if (myNbrState.UAV_ID == 6)
        {
            /* code */
            myNbrState.UAV_ID = 1;
        }
        
        usleep(5e4);
        myInterface.setNbrUAV(myNbrState);
        myNbrState.UAV_ID++;
    }
    
    myInterface.setNbrUAV(myNbrState);
    myInterface.setNbrUAV(myNbrState);
}
