#ifndef MISSION_CONTROL_SL
#define MISSION_CONTROL_SL

#include <thread>
#include <map>
#include <memory>
#include "msgQueue.hpp"
#include "swarmBasicData.h"
#include <vector>

class SwarmControlInterface;

class SimulinkIPC
{
public:
    SimulinkIPC();
    ~SimulinkIPC();

    void runThreads(); //启动后台运算线程

    void SetNbrUAVState(RealUAVStateBus &);

    void setSwarmInterface(SwarmControlInterface *);

private:
    std::map<std::string, unsigned int> priority{{"IO", 8192}, {"Snd", 16384}, {"Rcv", 0}};

    std::shared_ptr<msgQueue> ptrPosixMQ_Start;
    std::shared_ptr<msgQueue> ptrPosixMQ_ExtU;
    std::shared_ptr<msgQueue> ptrPosixMQ_ExtY;
    std::shared_ptr<msgQueue> ptrPosixMQ_SndCMD;
    std::shared_ptr<msgQueue> ptrPosixMQ_RcvCMD;
    std::shared_ptr<msgQueue> ptrPosixMQ_NbrState;

    void sndSimulinkInput();
    void rcvSimulinkOutput();
    void rlseAlgStart();
    void sndMissionCmd();
    void rcvMissionCmdFB();

    SwarmControlInterface *swarmInter;

    std::vector<std::thread *> ptrThreadPool;

    void batchJoin()
    {
        for (auto ptrThread : ptrThreadPool)
        {
            ptrThread->join();
        }
    }
};

#endif
