#pragma once
#include "ScenarioRunner.h"
#include "Autonomy.h"
#include <string>
#include <vector>

enum class Verdict {
    PASS,
    FAIL,
    UNCERTAIN
};

struct Result {
    std::string id;
    Verdict verdict;
    std::string info;
};

// helpers

bool insideFence(const Geofence& fence, Vec2 position);
const char* toString(Verdict v);


// requirements

Result checkSpeed(const std::vector<LogRow>& logs, double speed_limit_mps);
Result checkLocation(const std::vector<LogRow>& logs, Geofence fence);
Result checkWaypoints(const std::vector<LogRow>& logs, const std::vector<Vec2>& waypoints, double waypoint_tolerance_m);

std::string writeResults(const std::vector<Result>& results);

std::vector<Result> evaluate(const std::vector<LogRow>& logs, const Scenario& s);