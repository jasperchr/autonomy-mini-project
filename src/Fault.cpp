#include "Fault.h"

Fault::Fault(std::unique_ptr<VehicleInterface> vehicle, const SimClock& simClock, double startTime, double endTime) {
    vehicle_ = std::move(vehicle);
    simClock_ = &simClock;
    startTime_ = startTime;
    endTime_ = endTime;

}

bool Fault::sendCommand(const VehicleCommand& command) {
    return vehicle_->sendCommand(command);
}

std::optional<VehicleState> Fault::getState() const {
    return vehicle_->getState();
}

VehicleHealth Fault::getHealth() const {
    return vehicle_->getHealth();
}

bool Fault::isActive() const{
    return simClock_->now >= startTime_ && simClock_->now < endTime_;
}

// state dropout fault

std::optional<VehicleState> StateDropoutFault::getState() const {
    if (isActive()) {
        return std::nullopt;
    }
    return vehicle_->getState();
}

VehicleHealth StateDropoutFault::getHealth() const {
    VehicleHealth health = vehicle_->getHealth();
    if (isActive()) {
        health.has_Telemetry = false;
    }
    return health;
}
