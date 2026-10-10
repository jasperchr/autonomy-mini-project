#pragma once
#include "VehicleTypes.h"
#include <vector>
#include <string>

struct Scenario {
    std::string name{"default"};
    VehicleState initial_state{};
    std::vector<Vec2> waypoints{{50,0},{100,0},{150,0}};
    double duration_s{90.0};
    double dt_s{0.1};
    std::uint32_t seed{1};
};