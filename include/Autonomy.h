#pragma once
#include <vector>
#include "VehicleTypes.h"

double distance(const Vec2& a, const Vec2& b);

class Autonomy {
public:
    explicit Autonomy(std::vector<Vec2> waypoints);
    VehicleCommand autoPilot(const VehicleState& state);

private:
    VehicleCommand driveToWaypoint(const VehicleState& state, Vec2 waypoint);
    std::vector<Vec2> waypoints_{};
    size_t current_waypoint{0};

};