#include "MockRosSimulator.h"
#include <cassert>

MockRosSimulator::MockRosSimulator(const VehicleState& initialState, const SimulationParams& params, std::uint32_t seed){
    state_ = initialState;
    params_ = params;
    rng_ = std::mt19937(seed);
    noise_dist_ = std::normal_distribution<double>(0.0, params_.simulation_noise_m);
    latestState_ = initialState;

    assert(params_.telemetry_rate_hz > 0.0);
    telemetry_time_ = 1.0 / params_.telemetry_rate_hz;
}

void MockRosSimulator::command(const VehicleCommand& command) {
    command_ = command;
}

void MockRosSimulator::update(double dt) {
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

        latestState_ = noisyState;
        telemetry_time_ += 1.0 / params_.telemetry_rate_hz;
    }

}

std::optional<VehicleState> MockRosSimulator::getLatestState() const {
    return latestState_;
}

double MockRosSimulator::getTime() const {
    return time_;
}

VehicleState MockRosSimulator::SimState() const {
    return state_;
}