#pragma once
#include <vector>
#include <optional>
#include "VehicleTypes.h"

double distance(const Vec2& a, const Vec2& b);

class Autonomy {
public:
    explicit Autonomy(std::vector<Vec2> waypoints);
    VehicleCommand autoPilot(const std::optional<VehicleState>& state, double time_s);

private:
    VehicleCommand driveToWaypoint(const VehicleState& state, Vec2 waypoint);
    std::vector<Vec2> waypoints_{};
    size_t current_waypoint{0};

    // safe state vars
    double connection_timeout_s{5.0};
    double last_connection_s{};
    VehicleCommand lastCommand;
    double lastHeading;

};