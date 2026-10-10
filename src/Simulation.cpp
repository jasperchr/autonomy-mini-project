#include "Simulation.h"
#include <algorithm>
#include <cmath>

void advanceState(VehicleState& state, const VehicleCommand& command, double dt, const Limits& limits) {

    // update heading
    double maxTurnChange = limits.max_turn_rad_s * dt;
    double error = std::remainder(command.target_heading_rad - state.heading_rad, 2.0 * M_PI);
    double newHeading = state.heading_rad + std::clamp(error, -maxTurnChange, maxTurnChange);
    state.heading_rad = std::remainder(newHeading, 2.0 * M_PI);

    // update speed
    double maxSpeedChange = limits.max_accel_mps2 * dt;
    double newSpeed = state.speed_mps + std::clamp(command.target_speed_mps - state.speed_mps, -maxSpeedChange, maxSpeedChange);
    state.speed_mps = std::clamp(newSpeed, 0.0, limits.max_speed_mps);

    // update position
    state.position.x += state.speed_mps * std::cos(state.heading_rad) * dt;
    state.position.y += state.speed_mps * std::sin(state.heading_rad) * dt;
    

}