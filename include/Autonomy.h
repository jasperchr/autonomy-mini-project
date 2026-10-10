#pragma once
#include <vector>
#include "VehicleTypes.h"

class Autonomy {
public:
    explicit Autonomy(std::vector<Vec2> waypoints);
    VehicleCommand autoPilot(const VehicleState& state);

private:
    VehicleCommand driveToWaypoint(const VehicleState& state, Vec2 waypoint);
    std::vector<Vec2> waypoints_{};
    int current_waypoint{0};

};