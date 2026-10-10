#pragma once
#include <cstdint>

// 2d vector for position
struct Vec2 {
    double x{};
    double y{};
};

// base state of vehicle
struct VehicleState {
    Vec2 position;
    double heading_rad{};
    double speed_mps{};
    std::uint64_t timestamp{};
};

// basic autopilot command for vehicle
struct VehicleCommand {
    double target_speed_mps{};
    double target_heading_rad{};
};

// health check 
struct VehicleHealth {
    bool is_Connected{false};
    bool has_Telemetry{false};
};