#include "Simulation.h"
#include <algorithm>
#include <cmath>

static void applyWind(VehicleState& state, double dt, const SimulationParams& params) {

    const double t = state.timestamp / 1000.0;
    const double gust_speed_multiplier = 0.25;
    const double direction_variation_rad = 0.15;
    
    // wind gusts with changing speed and direction
    double speed = params.wind_speed_mps * (1.0 + gust_speed_multiplier * std::sin(2.0 * M_PI * t / params.wind_speed_period_s));
    double dir   = params.wind_direction_rad + direction_variation_rad * std::sin(2.0 * M_PI * t / params.wind_direction_period_s);

    state.position.x += speed * std::cos(dir) * dt;
    state.position.y += speed * std::sin(dir) * dt;
}

void advanceState(VehicleState& state, const VehicleCommand& command, double dt, const SimulationParams& params) {

    // update heading
    double maxTurnChange = params.limits.max_turn_rad_s * dt;
    double error = std::remainder(command.target_heading_rad - state.heading_rad, 2.0 * M_PI);
    double newHeading = state.heading_rad + std::clamp(error, -maxTurnChange, maxTurnChange);
    state.heading_rad = std::remainder(newHeading, 2.0 * M_PI);

    // update speed
    double maxSpeedChange = params.limits.max_accel_mps2 * dt;
    double newSpeed = state.speed_mps + std::clamp(command.target_speed_mps - state.speed_mps, -maxSpeedChange, maxSpeedChange);
    state.speed_mps = std::clamp(newSpeed, 0.0, params.limits.max_speed_mps);

    // update position
    state.position.x += state.speed_mps * std::cos(state.heading_rad) * dt;
    state.position.y += state.speed_mps * std::sin(state.heading_rad) * dt;

    applyWind(state, dt, params);

}