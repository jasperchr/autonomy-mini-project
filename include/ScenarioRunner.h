#pragma once
#include "Scenario.h"
#include "ScenarioLog.h"
#include "SimulationInterface.h"
#include "VehicleInterface.h"

std::vector<LogRow> run(const Scenario& s, VehicleInterface& vehicle, SimulationInterface& sim);
