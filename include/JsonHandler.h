#pragma once
#include "Scenario.h"
#include "Simulation.h"
#include <string>

struct Test {
    std::string source;
    Scenario scenario;
    SimulationParams ros_params;
    SimulationParams mavlink_params;
};

Test loadfromJson(const std::string& filePath);