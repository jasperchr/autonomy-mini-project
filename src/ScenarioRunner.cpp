#include "ScenarioRunner.h"
#include "Autonomy.h"
#include <cmath>

void run(const Scenario& s, VehicleInterface& vehicle, SimulationInterface& sim) {
    Autonomy a(s.waypoints);
    // how many iterations the loop needs to run (duration of scenario divided by tick rate)
    int total_steps = std::round(s.duration_s / s.dt_s);

    for (int k{0}; k < total_steps; k++){ 
        std::optional<VehicleState> currentState = vehicle.getState();
        if (currentState) {
            VehicleCommand cmd = a.autoPilot(*currentState);
            vehicle.sendCommand(cmd);
        }
        sim.update(s.dt_s);
    }
}
