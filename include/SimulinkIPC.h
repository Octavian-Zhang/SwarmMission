#ifndef MISSION_CIRCLE_H
#define MISSION_CIRCLE_H

#include <thread>
#include <map>
#include <memory>
#include "SwarmControlInterface.h"
#include "msgQueue.hpp"

class SimulinkIPC
{
public:
    SimulinkIPC();
    ~SimulinkIPC();

    void runThreads(); //启动后台运算线程
    void sndSimulinkInput();
    void rcvSimulinkOutput();
    void rlseAlgStart();
    void sndMissionCmd();
    void rcvMissionCmdFB();

    void setSwarmInterface(SwarmControlInterface *swarmInterface)
    {
        swarmInter = swarmInterface;
    }

private:
    std::map<std::string, unsigned int> priority{{"IO", 8192}, {"Snd", 16384}, {"Rcv", 1}};

    std::shared_ptr<msgQueue> ptrPosixMQ_Start;
    std::shared_ptr<msgQueue> ptrPosixMQ_ExtU;
    std::shared_ptr<msgQueue> ptrPosixMQ_ExtY;
    std::shared_ptr<msgQueue> ptrPosixMQ_SndCMD;
    std::shared_ptr<msgQueue> ptrPosixMQ_RcvCMD;

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
