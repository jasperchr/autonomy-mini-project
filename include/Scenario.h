#pragma once
#include "VehicleTypes.h"
#include <vector>
#include <string>
#include <optional>

struct Geofence {
    double x_min, x_max, y_min, y_max;
};

struct Scenario {
    std::string name{"default"};
    VehicleState initial_state{};
    std::vector<Vec2> waypoints{{50,0},{100,0},{150,0}};
    double duration_s{90.0};
    double dt_s{0.1};
    std::uint32_t seed{1};

    // Requirements
    std::optional<double> speed_limit_mps;
    std::optional<Geofence> geofence;
    std::optional<double> waypoint_tolerance_m;

    // Faults
    std::optional<double> dropout_start_s;
    std::optional<double> dropout_end_s;
    

};