#include <iostream>
#include <errno.h>
#include "SimulinkIPC.h"

SimulinkIPC::SimulinkIPC()
    : TriggerStart{new msgQueue("/PosixMQ_Start", O_CREAT | O_WRONLY, 1, sizeof(bool))}
    , RcvIndividualUAVCmd{new msgQueue("/PosixMQ_IndividualUAVCmd", O_CREAT | O_RDWR, 1, sizeof(IndividualUAVCmd))}
    , RcvFCUCMD{new msgQueue("/PosixMQ_FCUCMD", O_CREAT | O_RDONLY, 1, sizeof(FCUCMD))}
    , SndFlightMode{new msgQueue("/PosixMQ_FlightMode", O_CREAT | O_WRONLY, 1, sizeof(uint8_T))}
    , SndRealUAVStateBus{new msgQueue("/PosixMQ_RealUAVStateBus", O_CREAT | O_WRONLY, 1, sizeof(RealUAVStateBus))}
    , SndIndividualUAVCmd{new msgQueue("/PosixMQ_IndividualUAVCmd", O_CREAT | O_RDWR, 1, sizeof(IndividualUAVCmd))}
    , RcvTaskStatus{new msgQueue("/PosixMQ_TaskStatus", O_CREAT | O_RDONLY, 1, sizeof(TaskStatus))}
    , SndNbrState{new msgQueue("/PosixMQ_NbrUAVStateBus", O_CREAT | O_RDWR, 10, sizeof(RealUAVStateBus))}
    , SndVectorSpeed{new msgQueue("/PosixMQ_VectorSpeed", O_CREAT | O_WRONLY, 1, sizeof(VectorSpeed))}
{
}

void SimulinkIPC::rlseAlgStart()
{
    printf("Wait for time calibration\n");
    bool startflag{true};
    if (!(mq_send(TriggerStart->getMQ(), (char *)&startflag, sizeof(bool), priority["Low"]) < 0))
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

void SimulinkIPC::rcvMissionCmdFB(IndividualUAVCmd& fromSimulinkCMD)
{
    size_t msg_len = sizeof(fromSimulinkCMD);
    if (!(mq_receive(RcvIndividualUAVCmd->getMQ(), (char *)&fromSimulinkCMD, msg_len, &priority["Low"]) < 0))
    {
        std::cout << "********** Received CmdFB **********"
                  << std::endl << std::flush;
        // Parse Command Feedback here
    }
    else
    {
        int err = errno;
        std::cout << "Receive CmdFB errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::rcvFlightCMD(FCUCMD& FlightCMD)
{
    size_t msg_len = sizeof(FlightCMD);
    if (!(mq_receive(RcvFCUCMD->getMQ(), (char *)&FlightCMD, msg_len, &priority["High"]) < 0))
    {
        std::cout << ">" << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "Receive FlightCMD errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::sndFlightMode(const uint8_T FlightMode)
{
    size_t msg_len = sizeof(FlightMode);

    if (!(mq_send(SndFlightMode->getMQ(), (char *)&FlightMode, msg_len, priority["Hight"]) < 0))
    {
        std::cout << "+" << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "ExtU errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::SndFlightState(const RealUAVStateBus& FlightState)
{
    size_t msg_len = sizeof(FlightState);

    if (!(mq_send(SndRealUAVStateBus->getMQ(), (char *)&FlightState, msg_len, priority["Hight"]) < 0))
    {
        std::cout << "<" << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "ExtU errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::sndMissionCmd(const IndividualUAVCmd& toSimulinkCMD)
{
    size_t msg_len = sizeof(toSimulinkCMD);

    // Parse command input here
    if (!(mq_send(SndIndividualUAVCmd->getMQ(), (char *)&toSimulinkCMD, msg_len, priority["Hight"]) < 0))
    {
        std::cout << "********** Delivered FlightCMD **********" << std::endl << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "Deliver MissionCMD errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::rcvTaskStatus(TaskStatus& TaskState)
{
    size_t msg_len = sizeof(TaskState);
    if (!(mq_receive(RcvTaskStatus->getMQ(), (char *)&TaskState, msg_len, &priority["Low"]) < 0))
    {
        std::cout << "^" << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "Receive TaskState errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::SetNbrUAVState(const RealUAVStateBus &NbrState)
{
    mq_attr attr;
    if (!mq_getattr(SndNbrState->getMQ(), &attr) && attr.mq_curmsgs == attr.mq_maxmsg)
    {   // Queue is full
        RealUAVStateBus oldest{};
        if (mq_receive(SndNbrState->getMQ(), (char *)&oldest, sizeof(RealUAVStateBus), &priority["IO"]) >= 0)
        {
            std::cout << "Remove oldest neighbour state " << oldest.UAV_ID << std::endl << std::flush;
        }
    }
    
    if (!(mq_send(SndNbrState->getMQ(), (char *)&NbrState, sizeof(RealUAVStateBus), priority["IO"]) < 0))
    {
        std::cout << " UAV" << NbrState.UAV_ID << " " << std::endl << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "Neighbor UAV errno " << err << std::endl
                  << std::flush;
    }
}

void SimulinkIPC::sndVecSpd(const VectorSpeed& VecSpd)
{
    size_t msg_len = sizeof(VecSpd);

    if (!(mq_send(SndVectorSpeed->getMQ(), (char *)&VecSpd, msg_len, priority["Low"]) < 0))
    {
        std::cout << "o" << std::flush;
    }
    else
    {
        int err = errno;
        std::cout << "Send VectorSpeed errno " << err << std::endl
                  << std::flush;
    }
}