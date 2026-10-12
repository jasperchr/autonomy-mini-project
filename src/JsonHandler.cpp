#include "JsonHandler.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static Geofence parseGeofence(const json& payload) {
    Geofence g{};

    payload.at("x_min").get_to(g.x_min);
    payload.at("x_max").get_to(g.x_max);
    payload.at("y_min").get_to(g.y_min);
    payload.at("y_max").get_to(g.y_max);

    return g;
}

static Scenario loadScenarioFromJson(const json& payload) {
    Scenario s;

    payload.at("name").get_to(s.name);
    payload.at("duration_s").get_to(s.duration_s);
    payload.at("seed").get_to(s.seed);

    s.waypoints.clear();
    for (const json& wp : payload.at("waypoints")) {
        s.waypoints.push_back({wp.at(0).get<double>(), wp.at(1).get<double>()});
    }

    const json& criteria = payload.at("acceptance_criteria");

    if (criteria.contains("speed_limit_mps")) {
        s.speed_limit_mps = criteria.at("speed_limit_mps").get<double>();
    }

    if (criteria.contains("waypoint_tolerance_m")) {
        s.waypoint_tolerance_m = criteria.at("waypoint_tolerance_m").get<double>();
    }

    if (criteria.contains("geofence")) {
        s.geofence = parseGeofence(criteria.at("geofence"));
    }

    if (payload.contains("fault_triggers")) {
        const json& faults = payload.at("fault_triggers");

        if (faults.contains("dropout_start_s") && faults.contains("dropout_end_s")) {
            s.dropout_start_s = faults.at("dropout_start_s").get<double>();
            s.dropout_end_s = faults.at("dropout_end_s").get<double>();
        }
    }
   
    return s;

}

static SimulationParams loadParamsFromJson(const json& payload) {
    SimulationParams params;
    payload.at("telemetry_rate_hz").get_to(params.telemetry_rate_hz);
    payload.at("simulation_noise_m").get_to(params.simulation_noise_m);


    // wind simulation
    payload.at("wind_speed_mps").get_to(params.wind_speed_mps);
    payload.at("wind_direction_rad").get_to(params.wind_direction_rad);
    payload.at("wind_speed_period_s").get_to(params.wind_speed_period_s);
    payload.at("wind_direction_period_s").get_to(params.wind_direction_period_s);

    // sensor error simulation
    payload.at("gps_bias_m").get_to(params.gps_bias_m);
    payload.at("gps_bias_time_constant_s").get_to(params.gps_bias_time_constant_s);
    payload.at("speed_noise_mps").get_to(params.speed_noise_mps);
    payload.at("heading_noise_rad").get_to(params.heading_noise_rad);

    // vehicle limits
    const json& limits = payload.at("limits");
    limits.at("max_speed_mps").get_to(params.limits.max_speed_mps);
    limits.at("max_turn_rad_s").get_to(params.limits.max_turn_rad_s);
    limits.at("max_accel_mps2").get_to(params.limits.max_accel_mps2);
    limits.at("speed_time_constant_s").get_to(params.limits.speed_time_constant_s);
    limits.at("heading_time_constant_s").get_to(params.limits.heading_time_constant_s);

    return params;
}

Test loadfromJson(const std::string& filePath) {

    std::ifstream f(filePath);

    if (!f.is_open()) {
        throw std::runtime_error("json cannot be opened");
    }

    const json& payload = json::parse(f);

    Test t;

    const json& vehicle = payload.at("vehicles");
    t.source = filePath;
    t.scenario = loadScenarioFromJson(payload);
    t.ros_params = loadParamsFromJson(vehicle.at("ros"));
    t.mavlink_params = loadParamsFromJson(vehicle.at("mavlink"));
    
    return t;
}