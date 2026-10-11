#pragma once
#include "Scenario.h"
#include "ScenarioLog.h"
#include "SimulationInterface.h"
#include "VehicleInterface.h"
#include "Fault.h"

std::vector<LogRow> run(const Scenario& s, VehicleInterface& vehicle, SimulationInterface& sim, SimClock& clock);

std::unique_ptr<VehicleInterface> runFaults(std::unique_ptr<VehicleInterface> vehicle, const Scenario& s, const SimClock& clock);
