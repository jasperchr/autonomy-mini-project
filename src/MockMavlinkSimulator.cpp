#include "MockMavlinkSimulator.h"
#include <cassert>
#include <cmath>

static std::uint16_t radToCdeg (double heading_rad) {
    double bearing_deg = 90.0 - heading_rad * 180.0 / M_PI;
    double wrapped_deg = bearing_deg - 360.0 * std::floor(bearing_deg / 360.0);
    return static_cast<std::uint16_t>(std::lround(wrapped_deg * 100.0) % 36000);
}

static MavlinkState toMavlinkState (const VehicleState& state) {
    MavlinkState mav_state;

    mav_state.pos_north_m = state.position.y;
    mav_state.pos_east_m = state.position.x;

    mav_state.heading_cdeg = radToCdeg(state.heading_rad);

    mav_state.velocity_north_mps = state.speed_mps * sin(state.heading_rad);
    mav_state.velocity_east_mps = state.speed_mps * cos(state.heading_rad);

    mav_state.time_since_boot_ms = state.timestamp;

    return mav_state;
}

MockMavlinkSimulator::MockMavlinkSimulator(const VehicleState& initialState, const SimulationParams& params, std::uint32_t seed){
    state_ = initialState;
    params_ = params;
    rng_ = std::mt19937(seed);
    noise_dist_ = std::normal_distribution<double>(0.0, params_.simulation_noise_m);
    latestState_ = toMavlinkState(initialState);

    assert(params_.telemetry_rate_hz > 0.0);
    telemetry_time_ = 1.0 / params_.telemetry_rate_hz;
}

void MockMavlinkSimulator::command(const MavlinkCommand& command) {
    command_.target_speed_mps = std::hypot(command.target_velocity_north_mps, command.target_velocity_east_mps);
    if (command_.target_speed_mps > 0) {
        command_.target_heading_rad = atan2(command.target_velocity_north_mps, command.target_velocity_east_mps);
    } else command_.target_heading_rad = state_.heading_rad;
}

void MockMavlinkSimulator::update(double dt) {
    time_ += dt;
    advanceState(state_, command_, dt, params_);

    // set timestamp
    state_.timestamp = static_cast<std::uint64_t>(std::round(time_ * 1000.0));

    // add noise to the state for telemetry and take a new sample
    telemetry_time_ -= dt;
    if (telemetry_time_ <= 1e-9) {
        VehicleState noisyState = state_;
        noisyState.position.x += noise_dist_(rng_);
        noisyState.position.y += noise_dist_(rng_);

        latestState_ = toMavlinkState(noisyState);
        telemetry_time_ += 1.0 / params_.telemetry_rate_hz;
    }

}

std::optional<MavlinkState> MockMavlinkSimulator::getLatestState() const {
    return latestState_;
}

double MockMavlinkSimulator::getTime() const {
    return time_;
}

VehicleState MockMavlinkSimulator::SimState() const {
    return state_;
}