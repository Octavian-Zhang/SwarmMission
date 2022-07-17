#ifndef MAVLINK_SWARM_CONTROL_INTERFACE_H
#define MAVLINK_SWARM_CONTROL_INTERFACE_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include "swarmBasicData.h"
#include "SimulinkIPC.h"

class SimulinkIPC;

class SwarmControlInterface
{
public:
    SwarmControlInterface() {}
    ~SwarmControlInterface() {}

    // TimeCalibrated
    void calibrateTime()                                            // Call by DataReactor
    {
        std::unique_lock<std::mutex> lk(mutexTimeCalibrated);
        TimeCalibrated = true;
        lk.unlock();
        cvTimeCalibrated.notify_one();
    }
    void waitTillCalibrated()                                       // Call by Algorithm, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexTimeCalibrated);
        cvTimeCalibrated.wait(lk, [&]()
                              { return this->TimeCalibrated; });
    }

    // FCUCMD, 0x92
    const FCUCMD &getFlightCMD()                                       // Call by DataReactor, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexFlightCMD);
        cvFlightCMD.wait(lk, [&]()
                      { return this->hasFlightCMD; });
        hasFlightCMD = false;
        return this->FlightCMD;
    }
    void setFlightCMD(const FCUCMD &refFlightCMD)                         // Call by Algorithm
    {
        std::unique_lock<std::mutex> lk(mutexFlightCMD);
        this->FlightCMD = refFlightCMD;
        hasFlightCMD = true;
        lk.unlock();
        cvFlightCMD.notify_one();
    }

    // FlightMode
    const uint8_T getFlightMode()                                   // Call by Algorithm
    {
        return this->FlightMode;
    }
    void setFlightMode(const uint8_T &refFlightMode)                // Call by DataReactor, PointingMode == 3
    {
        this->FlightMode = refFlightMode;
    }

    // StateFCU
    const StateFCU &getStateFCU()                                   // Call by Algorithm
    {
        const std::lock_guard<std::mutex> lock(mutexFlightState);
        return this->FlightState;
    }
    void setStateFCU(const StateFCU &refStateFCU)                   // Call by DataReactor
    {
        const std::lock_guard<std::mutex> lock(mutexFlightState);
        this->FlightState = refStateFCU;
    }

    // TaskStatus, 0xC2
    const TaskStatus &getTaskStatus()                               // Call by DataReactor, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexTaskStatus);
        cvTaskStatus.wait(lk, [&]()
                          { return this->hasTaskStatus; });
        hasTaskStatus = false;
        return this->MissionStatusFB;
    }
    void setTaskStatus(const TaskStatus &refTaskStatus)             // Call by Algorithm
    {
        std::unique_lock<std::mutex> lk(mutexTaskStatus);
        this->MissionStatusFB = refTaskStatus;
        hasTaskStatus = true;
        lk.unlock();
        cvTaskStatus.notify_one();
    }

    // GCSCMD
    IndividualUAVCmd &getGCSCMD()                                   // Call by Algorithm, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexGCSCMD);
        cvGCSCMD.wait(lk, [&]()
                      { return this->hasGCSCMD; });
        hasGCSCMD = false;
        return this->GCSCMD;
    }
    void setGCSCMD(IndividualUAVCmd& refGCSCMD)                     // Call by DataReactor
    {
        std::unique_lock<std::mutex> lk(mutexGCSCMD);
        this->GCSCMD = refGCSCMD;
        hasGCSCMD = true;
        lk.unlock();
        cvGCSCMD.notify_one();
    }

    friend void SimulinkIPC::setSwarmInterface(SwarmControlInterface *swarmInterface);

    void setNbrUAV(RealUAVStateBus &NbrState)                       // Call by DataReactor
    {
        if (nullptr != slIPC)
        {
            slIPC->SetNbrUAVState(NbrState);
        }
    }

private:

    bool TimeCalibrated{false};
    std::mutex mutexTimeCalibrated;    // Asynchronous
    std::condition_variable cvTimeCalibrated;

    FCUCMD FlightCMD{};      // 0x92, MissionControl -> FlightControl
    std::mutex mutexFlightCMD; // 10 Herz
    std::condition_variable cvFlightCMD;
    bool hasFlightCMD{false};

    std::atomic<uint8_T> FlightMode{};   // Pointing Mode -> FlightMode == 3, 10 Herz

    StateFCU FlightState{};    // 0xC2, FlightControl -> MissionControl
    std::mutex mutexFlightState; // 10 Herz

    TaskStatus MissionStatusFB{};  // 0xC2, MissionControl -> GroundControl
    std::mutex mutexTaskStatus; // 10 Herz
    std::condition_variable cvTaskStatus;
    bool hasTaskStatus{false};

    IndividualUAVCmd GCSCMD{}; // 0xFC, GroundControl -> MissionControl
    std::mutex mutexGCSCMD;    // Asynchronous
    std::condition_variable cvGCSCMD;
    bool hasGCSCMD{false};

    SimulinkIPC *slIPC{};
    void setIPC(SimulinkIPC *ptrIPC)
    {
        slIPC = ptrIPC;
    }
};

#endif
