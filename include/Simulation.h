#pragma once
#include "VehicleTypes.h"
#include <random>

struct Limits {
    double max_speed_mps{25.0};
    double max_turn_rad_s{5.0};
    double max_accel_mps2{5.0};

    // first order lag simulation - 0.1 means no lag as this matches dt
    double speed_time_constant_s{0.1};
    double heading_time_constant_s{0.1};
};

struct SimulationParams {
    Limits limits{};
    double simulation_noise_m{0.5};
    double telemetry_rate_hz{10.0};

    // wind simulation
    double wind_speed_mps{0.0};
    double wind_direction_rad{0.0};
    double wind_speed_period_s{50.0};
    double wind_direction_period_s{71.0};

    // sensor error simulation
    double gps_bias_m{0.0};
    double gps_bias_time_constant_s{30.0};
    double speed_noise_mps{0.0};
    double heading_noise_rad{0.0};
};

void advanceState(VehicleState& state, const VehicleCommand& command, double dt, const SimulationParams& params);

void applySensorErrors(VehicleState& state, Vec2& position_bias, std::mt19937& rng, double dt, const SimulationParams& params);