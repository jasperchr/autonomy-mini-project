#include "Simulation.h"
#include <algorithm>
#include <cmath>

// basic wind simulation 
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

// first order lag
static void applyLag(VehicleState& state, const VehicleCommand& command, double dt, const SimulationParams& params) {

    // close a fraction of the "gap" between current and target speed / heading every tick
    // capped by accel / rate limits and wrapped
    double speed_gap = command.target_speed_mps - state.speed_mps;
    double speed_fraction = dt / params.limits.speed_time_constant_s;
    double speed_step = speed_gap * speed_fraction;
    state.speed_mps += std::clamp(speed_step, -(params.limits.max_accel_mps2 * dt), params.limits.max_accel_mps2 * dt);
    state.speed_mps = std::clamp(state.speed_mps, 0.0, params.limits.max_speed_mps);

    double heading_gap = std::remainder((command.target_heading_rad - state.heading_rad), 2.0 * M_PI);
    double heading_fraction = dt / params.limits.heading_time_constant_s;
    double heading_step = heading_gap * heading_fraction;
    state.heading_rad += std::clamp(heading_step, -(params.limits.max_turn_rad_s * dt), params.limits.max_turn_rad_s * dt);
    state.heading_rad = std::remainder(state.heading_rad, 2.0 * M_PI);
}

void advanceState(VehicleState& state, const VehicleCommand& command, double dt, const SimulationParams& params) {

    // update our speed and heading with first order lag 
    applyLag(state, command, dt, params);

    // update position
    state.position.x += state.speed_mps * std::cos(state.heading_rad) * dt;
    state.position.y += state.speed_mps * std::sin(state.heading_rad) * dt;

    applyWind(state, dt, params);

}