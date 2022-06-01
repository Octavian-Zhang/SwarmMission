#ifndef MAVLINK_SWARM_CONTROL_INTERFACE_H
#define MAVLINK_SWARM_CONTROL_INTERFACE_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include "swarmBasicData.h"

class SwarmControlInterface
{
public:
    SwarmControlInterface() {}
    ~SwarmControlInterface() {}

    bool StartMission{false};

    const uint32_t getSysID() { return this->sysID; }               // Call by Algorithm
    void setSysID(const uint32_t &sysID) { this->sysID = sysID; }   // Call by DataReactor

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
    const FCUCMD &getFCUCMD()                                       // Call by DataReactor, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexFCUCMD);
        cvFCUCMD.wait(lk, [&]()
                      { return this->hasFCUCMD; });
        hasFCUCMD = false;
        return this->FCUCMD_i;
    }
    void setFCUCMD(const FCUCMD &refFCUCMD)                         // Call by Algorithm
    {
        std::unique_lock<std::mutex> lk(mutexFCUCMD);
        this->FCUCMD_i = refFCUCMD;
        hasFCUCMD = true;
        lk.unlock();
        cvFCUCMD.notify_one();
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
        const std::lock_guard<std::mutex> lock(mutexStateFCU);
        return this->StateFCU_i;
    }
    void setStateFCU(const StateFCU &refStateFCU)                   // Call by DataReactor
    {
        const std::lock_guard<std::mutex> lock(mutexStateFCU);
        this->StateFCU_i = refStateFCU;
    }

    // TaskStatus, 0xC2
    const TaskStatus &getTaskStatus()                               // Call by DataReactor, block till new data available
    {
        std::unique_lock<std::mutex> lk(mutexTaskStatus);
        cvTaskStatus.wait(lk, [&]()
                          { return this->hasTaskStatus; });
        hasTaskStatus = false;
        return this->TaskStatus_i;
    }
    void setTaskStatus(const TaskStatus &refTaskStatus)             // Call by Algorithm
    {
        std::unique_lock<std::mutex> lk(mutexTaskStatus);
        this->TaskStatus_i = refTaskStatus;
        hasTaskStatus = true;
        lk.unlock();
        cvTaskStatus.notify_one();
    }

    // OtherUAVstate
    void getOtherUAVstate(std::array<RealUAVStateBus, 128> &NeighbourStates)    // Call by Algorithm
    {
        const std::lock_guard<std::mutex> lock(mutexOtherUAV);
        std::copy(std::begin(this->OtherUAVstate), std::end(this->OtherUAVstate), NeighbourStates.begin());
    }
    void setOtherUAVstate(std::array<RealUAVStateBus, 128> &NeighbourStates)    // Call by DataReactor
    {
        const std::lock_guard<std::mutex> lock(mutexOtherUAV);
        std::move(NeighbourStates.begin(), NeighbourStates.end(), std::begin(this->OtherUAVstate));
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

private:
    std::atomic<uint32_t> sysID; //本机ID

    bool TimeCalibrated{false};
    std::mutex mutexTimeCalibrated;    // Asynchronous
    std::condition_variable cvTimeCalibrated;

    FCUCMD FCUCMD_i{};      // 0x92, MissionControl -> FlightControl
    std::mutex mutexFCUCMD; // 10 Herz
    std::condition_variable cvFCUCMD;
    bool hasFCUCMD{false};

    std::atomic<uint8_T> FlightMode{};   // Pointing Mode -> FlightMode == 3, 10 Herz

    StateFCU StateFCU_i{};    // 0xC2, FlightControl -> MissionControl
    std::mutex mutexStateFCU; // 10 Herz

    TaskStatus TaskStatus_i{};  // 0xC2, MissionControl -> GroundControl
    std::mutex mutexTaskStatus; // 10 Herz
    std::condition_variable cvTaskStatus;
    bool hasTaskStatus{false};

    RealUAVStateBus OtherUAVstate[128]; // 0xC2, OtherUAV -> ThisUAV
    std::mutex mutexOtherUAV;

    IndividualUAVCmd GCSCMD{}; // 0xFC, GroundControl -> MissionControl
    std::mutex mutexGCSCMD;    // Asynchronous
    std::condition_variable cvGCSCMD;
    bool hasGCSCMD{false};

};

#endif
