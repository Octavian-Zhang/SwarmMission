#include <iostream>
#include "SimulinkIPC.h"
#include <errno.h>
#include "SwarmControlInterface.h"

SimulinkIPC::SimulinkIPC()
    : ptrPosixMQ_Start{new msgQueue("/PosixMQ_Start", O_CREAT | O_WRONLY, 1, sizeof(bool))}
    , ptrPosixMQ_ExtU{new msgQueue("/PosixMQ_ExtU", O_CREAT | O_WRONLY, 1, sizeof(ExtU_codegenReal2Mission_T))}
    , ptrPosixMQ_ExtY{new msgQueue("/PosixMQ_ExtY", O_CREAT | O_RDONLY, 1, sizeof(ExtY_codegenReal2Mission_T))}
    , ptrPosixMQ_SndCMD{new msgQueue("/PosixMQ_SndCMD", O_CREAT | O_RDONLY, 1, sizeof(IndividualUAVCmd))}
    , ptrPosixMQ_RcvCMD{new msgQueue("/PosixMQ_RcvCMD", O_CREAT | O_WRONLY, 1, sizeof(IndividualUAVCmd))}
    , ptrPosixMQ_NbrState{new msgQueue("/PosixMQ_NbrState", O_CREAT | O_WRONLY, 1, sizeof(RealUAVStateBus))}
{
}

void SimulinkIPC::runThreads()
{
    std::cout << "Simulink IO thread pool started\n"
              << std::endl;

    ptrThreadPool.push_back(new std::thread(&SimulinkIPC::sndSimulinkInput, this));
    ptrThreadPool.push_back(new std::thread(&SimulinkIPC::rcvSimulinkOutput, this));
    ptrThreadPool.push_back(new std::thread(&SimulinkIPC::sndMissionCmd, this));
    ptrThreadPool.push_back(new std::thread(&SimulinkIPC::rcvMissionCmdFB, this));
    ptrThreadPool.push_back(new std::thread(&SimulinkIPC::rlseAlgStart, this));
}

void SimulinkIPC::SetNbrUAVState(RealUAVStateBus &NbrState)
{
    if (!(mq_send(ptrPosixMQ_NbrState->getMQ(), (char *)&NbrState, sizeof(RealUAVStateBus), priority["IO"]) < 0))
    {
        std::cout << " UAV" << NbrState.UAV_ID << " " << std::endl;
    }
    else
    {
        int err = errno;
        std::cout << "Neighbor UAV errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::setSwarmInterface(SwarmControlInterface *swarmInterface)
{
    swarmInter = swarmInterface;
    swarmInter->setIPC(this);
}

void SimulinkIPC::rcvSimulinkOutput()
{
    ExtY_codegenReal2Mission_T ExtY{}; // forward declaration
    size_t msg_len = sizeof(ExtY);
    while (true)
    { // blocking queue
        if (!(mq_receive(ptrPosixMQ_ExtY->getMQ(), (char *)&ExtY, msg_len, &priority["IO"]) < 0))
        {
            std::cout << "Received ExtY"
                      << std::endl;
            // Parse Simulink Output here
            swarmInter->setFlightCMD(ExtY.FlightCMD);
            swarmInter->setTaskStatus(ExtY.MissionFB);
        }
        else
        {
            int err = errno;
            std::cout << "ExtY errno " << err << std::endl
                      << std::flush;
        }
    }
}

void SimulinkIPC::sndSimulinkInput()
{
    ExtU_codegenReal2Mission_T ExtU{}; // forward declaration
    size_t msg_len = sizeof(ExtU);
    while (true)
    {
        // Parse Simulink Input here
        ExtU.FlightMode = swarmInter->getFlightMode();
        ExtU.FlightState = swarmInter->getStateFCU();
        if (!(mq_send(ptrPosixMQ_ExtU->getMQ(), (char *)&ExtU, msg_len, priority["IO"]) < 0))
        {
            std::cout << "Sent ExtU"
                      << std::endl;
        }
        else
        {
            int err = errno;
            std::cout << "ExtU errno " << err << std::endl
                      << std::flush;
        }
    }
}

void SimulinkIPC::rlseAlgStart()
{
    printf("Wait for time calibration\n");
    swarmInter->waitTillCalibrated();
    bool startflag{true};
    unsigned int startflagprio = 1;
    if (!(mq_send(ptrPosixMQ_Start->getMQ(), (char *)&startflag, sizeof(bool), startflagprio) < 0))
    {
        printf("Time calibrated\n");
    }
    else
    {
        int err = errno;
        std::cout << "Start errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::sndMissionCmd()
{
    IndividualUAVCmd toSimulinkCMD{}; // forward declaration
    size_t msg_len = sizeof(toSimulinkCMD);
    while (true)
    {
        // Parse command input here
        toSimulinkCMD = swarmInter->getGCSCMD();
        mq_attr myAttr;
        ptrPosixMQ_RcvCMD->getAttr(&myAttr);
        if (!(mq_send(ptrPosixMQ_RcvCMD->getMQ(), (char *)&toSimulinkCMD, msg_len, priority["Snd"]) < 0))
        {
            std::cout << "********** Delivered RcvCMD **********" << std::endl;
        }
        else
        {
            int err = errno;
            std::cout << "Deliver RcvCMD errno " << err << std::endl
                      << std::flush;
        }
    }
}

void SimulinkIPC::rcvMissionCmdFB()
{
    IndividualUAVCmd fromSimulinkCMD{};
    size_t msg_len = sizeof(fromSimulinkCMD);
    while (true)
    {
        if (!(mq_receive(ptrPosixMQ_SndCMD->getMQ(), (char *)&fromSimulinkCMD, msg_len, &priority["Rcv"]) < 0))
        {
            std::cout << "********** Received SndCMD **********"
                      << std::endl;
            // Parse Command Feedback here
        }
        else
        {
            int err = errno;
            std::cout << "Receive SndCMD errno " << err << std::endl
                      << std::flush;
        }
    }
}

SimulinkIPC::~SimulinkIPC()
{
    batchJoin();
}