#include "MockRosVehicle.h"

MockRosVehicle::MockRosVehicle(std::unique_ptr<MockRosSimulator> simulator) {
    simulator_ = std::move(simulator);
}

bool MockRosVehicle::sendCommand(const VehicleCommand& command) {
    simulator_->command(command);
    return true;
}

std::optional<VehicleState> MockRosVehicle::getState() const {
    return simulator_->getLatestState();
}

VehicleHealth MockRosVehicle::getHealth() const {
    VehicleHealth health;
    health.is_Connected = true;
    health.has_Telemetry = true;
    return health;
}