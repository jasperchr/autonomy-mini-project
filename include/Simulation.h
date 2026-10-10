#pragma once
#include "VehicleTypes.h"

struct Limits {
    double max_speed_mps{25.0};
    double max_turn_rad_s{5.0};
    double max_accel_mps2{5.0};
};

struct SimulationParams {
    Limits limits{};
    double simulation_noise_m{0.5};
    double telemetry_rate_hz{10.0};
};

void advanceState(VehicleState& state, const VehicleCommand& command, double dt, const Limits& limits);