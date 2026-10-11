#include "Evaluator.h"

bool insideFence(const Geofence& fence, Vec2 position) {
     return position.x >= fence.x_min &&
            position.x <= fence.x_max &&
            position.y >= fence.y_min &&
            position.y <= fence.y_max;

}

const char* toString(Verdict v) {
    switch (v) {
        case Verdict::PASS:      return "PASS";
        case Verdict::FAIL:      return "FAIL";
        case Verdict::UNCERTAIN: return "UNCERTAIN";
    }
    return "UNKNOWN";
}

// requirements

// check if we breach speed limit during test run
Result checkSpeed(const std::vector<LogRow>& logs, double speed_limit_mps) {
    Result result;
    result.id = "001-SPEED";
    if(logs.empty()) {
        result.verdict = Verdict::UNCERTAIN;
        result.info = "UNCERTAIN: Cannot read any data from log file";
        return result;
    };

    for (const LogRow& l : logs) {
        if (l.actual_state.speed_mps > speed_limit_mps) {
            result.verdict = Verdict::FAIL;
            result.info = "FAIL: Speed limit exceeded, actual value " + std::to_string(l.actual_state.speed_mps) + " exceeds limit " + std::to_string(speed_limit_mps); 
            return result;
        }
    } 
    result.verdict = Verdict::PASS;
    result.info = "PASS: UxV within speed limit during test";
    return result;
}

// check if we breach geofence during test run
Result checkLocation(const std::vector<LogRow>& logs, Geofence fence) {
    Result result;
    result.id = "002-GEOFENCE";
    if(logs.empty()) {
        result.verdict = Verdict::UNCERTAIN;
        result.info = "UNCERTAIN: Cannot read any data from log file";
        return result;
    };
    for (const LogRow& l : logs) {
        if(!insideFence(fence, l.actual_state.position)) {
            result.verdict = Verdict::FAIL;
            result.info = "FAIL: Geofence breached, UxV outside of Geofence boundaries";
            return result;
        }
    }
    result.verdict = Verdict::PASS;
    result.info = "PASS: UxV inside Geofence during test";
    return result;
}

// check if we reach all the waypoints and in correct order
Result checkWaypoints(const std::vector<LogRow>& logs, const std::vector<Vec2>& waypoints, double waypoint_tolerance_m) {
    Result result;
    result.id = "003-WAYPOINTS";
    std::size_t wpIndex{0};
    if(logs.empty()) {
        result.verdict = Verdict::UNCERTAIN;
        result.info = "UNCERTAIN: Cannot read any data from log file";
        return result;
    };
    if(waypoints.empty()) {
        result.verdict = Verdict::UNCERTAIN;
        result.info = "UNCERTAIN: Cannot read any waypoints";
        return result;
    };
    for (const LogRow& l : logs) {
        if (distance(l.actual_state.position, waypoints[wpIndex]) <= waypoint_tolerance_m){
            wpIndex++;
        }
        if(wpIndex == waypoints.size()){
            result.verdict = Verdict::PASS;
            result.info = "PASS: UxV reached all waypoints";
            return result;
        }
    }
    result.verdict = Verdict::FAIL;
    result.info = "FAIL: UxV did not reach all waypoints or broke order";
    return result;
}

std::string writeResults(const std::vector<Result>& results) {
    std::string output;
    for (const Result& result : results) {
        output += "\nTEST ID - " + result.id + "\nVERDICT - " + toString(result.verdict) + "\nINFO - " + result.info;
    }
    return output;
}

std::vector<Result> evaluate(const std::vector<LogRow>& logs, const Scenario& s) {
    std::vector<Result> results;
    if(s.speed_limit_mps) {
        results.emplace_back(checkSpeed(logs,*s.speed_limit_mps));
    }
    if (s.geofence) {
        results.emplace_back(checkLocation(logs,*s.geofence));
    }
    if (s.waypoint_tolerance_m) {
        results.emplace_back(checkWaypoints(logs, s.waypoints, *s.waypoint_tolerance_m));
    }
    return results;
}