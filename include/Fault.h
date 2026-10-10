#pragma once
#include "VehicleInterface.h"
#include <memory>

struct SimClock {double now{0.0};};

class Fault: public VehicleInterface {
public:
    // constructor - takes pointer to vehicle and start/end times for fault
    Fault(std::unique_ptr<VehicleInterface> vehicle, const SimClock& simClock, double startTime, double endTime);

    // override VehicleInterface methods
    bool sendCommand(const VehicleCommand& command) override;
    std::optional<VehicleState> getState() const override;
    VehicleHealth getHealth() const override;

protected:
    // vehicle and fault info - child needs this
    std::unique_ptr<VehicleInterface> vehicle_;
    const SimClock& simClock_;
    double startTime_;
    double endTime_;
    // is the fault active?
    bool isActive() const;
};
