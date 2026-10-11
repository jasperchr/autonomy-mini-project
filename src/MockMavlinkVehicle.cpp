#include "MockMavlinkVehicle.h"
#include <cmath>

VehicleState toVehicleState (const MavlinkState& mav_state) {
    VehicleState state;

    state.position.x = mav_state.pos_east_m;
    state.position.y = mav_state.pos_north_m;

    state.heading_rad = (90 - mav_state.heading_cdeg / 100.0) * M_PI / 180;

    state.speed_mps = std::hypot(mav_state.velocity_north_mps, mav_state.velocity_east_mps);

    state.timestamp = mav_state.time_since_boot_ms;
    
    return state;
}

MavlinkCommand toMockMavCommand (const VehicleCommand& command) {
    MavlinkCommand mav_command;

    mav_command.target_velocity_north_mps = command.target_speed_mps * sin(command.target_heading_rad);
    mav_command.target_velocity_east_mps = command.target_speed_mps * cos(command.target_heading_rad);

    return mav_command;
}

MockMavlinkVehicle::MockMavlinkVehicle(std::unique_ptr<MockMavlinkSimulator> simulator) {
    simulator_ = std::move(simulator);
}

bool MockMavlinkVehicle::sendCommand(const VehicleCommand& command) {
    simulator_->command(toMockMavCommand(command));
    return true;
}

std::optional<VehicleState> MockMavlinkVehicle::getState() const {
    std::optional<MavlinkState> latestState = simulator_->getLatestState();
    if (latestState) {
        return toVehicleState(*latestState);
    }
    return std::nullopt;
}

VehicleHealth MockMavlinkVehicle::getHealth() const {
    VehicleHealth health;
    health.is_Connected = true;
    health.has_Telemetry = true;
    return health;
}