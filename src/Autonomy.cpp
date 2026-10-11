#include "Autonomy.h"
#include <cmath>
#include <algorithm>

double distance(const Vec2& a, const Vec2& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Autonomy::Autonomy(std::vector<Vec2> waypoints){
    waypoints_ = waypoints;
}

VehicleCommand Autonomy::driveToWaypoint(const VehicleState& state, Vec2 waypoint) {
    VehicleCommand command;

    double delta_x = waypoint.x - state.position.x;
    double delta_y = waypoint.y - state.position.y;

    // set target speed based on distance to waypoint, clamped between 2.0 and 10.0 m/s
    command.target_speed_mps = std::clamp(distance(state.position, waypoint) * 0.1, 2.0, 10.0);

    // set target heading towards the waypoint
    command.target_heading_rad = std::atan2(delta_y, delta_x);

    return command;
}

VehicleCommand Autonomy::autoPilot(const std::optional<VehicleState>& state, double time_s) {
    
    // define a stop command
    VehicleCommand stop;
    stop.target_speed_mps = 0.0;
    stop.target_heading_rad = lastHeading;

    // safe state handling
    if (!state) {
        if (time_s > last_connection_s + connection_timeout_s) {
            return stop;
        }
        return lastCommand;
    }

    lastHeading = state->heading_rad;
    last_connection_s = time_s;

    // if we have reached our current waypoint, move to the next one
    if (current_waypoint < waypoints_.size() && distance(state->position, waypoints_[current_waypoint]) < 3.0) {
        current_waypoint++;
    }

    // if we have no more waypoints, stop
    if (current_waypoint >= waypoints_.size()) {
        return stop;
    }

    // otherwise, drive to the current waypoint
    lastCommand = driveToWaypoint(*state, waypoints_[current_waypoint]);
    return lastCommand;
}