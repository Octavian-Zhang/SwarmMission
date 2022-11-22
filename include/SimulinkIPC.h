#ifndef MISSION_CONTROL_SL
#define MISSION_CONTROL_SL

#include <thread>
#include <map>
#include <memory>
#include <vector>
#include "msgQueue.hpp"
#include "DatalinkInterface.h"

class SimulinkIPC
{
public:
    SimulinkIPC();

    void rlseAlgStart();
    void rcvMissionCmdFB(IndividualUAVCmd &);
    void rcvFlightCMD(FCUCMD &);
    void sndFlightMode(const uint8_T);
    void SndFlightState(const RealUAVStateBus&);
    void sndMissionCmd(const IndividualUAVCmd&);
    void rcvTaskStatus(TaskStatus&);
    void SetNbrUAVState(const RealUAVStateBus&);
    void sndVecSpd(const VectorSpeed&);

private:
    std::map<std::string, unsigned int> priority{{"IO", 8192}, {"High", 16384}, {"Low", 0}};

    std::shared_ptr<msgQueue> TriggerStart;             // Arg0
    std::shared_ptr<msgQueue> RcvIndividualUAVCmd;      // Arg1
    std::shared_ptr<msgQueue> RcvFCUCMD;                // Arg2
    std::shared_ptr<msgQueue> SndFlightMode;            // Arg4
    std::shared_ptr<msgQueue> SndRealUAVStateBus;       // Arg5
    std::shared_ptr<msgQueue> SndIndividualUAVCmd;      // Arg6
    std::shared_ptr<msgQueue> RcvTaskStatus;            // Arg7
    std::shared_ptr<msgQueue> SndNbrState;              // Arg8
    std::shared_ptr<msgQueue> SndVectorSpeed;           // Arg9
};

#endif
