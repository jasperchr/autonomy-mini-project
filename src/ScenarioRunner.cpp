#include "ScenarioRunner.h"
#include "Autonomy.h"
#include <cmath>

std::vector<LogRow> run(const Scenario& s, VehicleInterface& vehicle, SimulationInterface& sim, SimClock& clock) {
    Autonomy a(s.waypoints);
    std::vector<LogRow> logs;
    
    // how many iterations the loop needs to run (duration of scenario divided by tick rate)
    int total_steps = std::round(s.duration_s / s.dt_s);

    for (int k{0}; k < total_steps; k++){ 
        clock.now = k * s.dt_s; // update fault timers
        std::optional<VehicleState> currentState = vehicle.getState();
        LogRow current_log;
        if (currentState) {
            VehicleCommand cmd = a.autoPilot(*currentState);
            vehicle.sendCommand(cmd);
            current_log.command = cmd;
        }

        sim.update(s.dt_s);

        current_log.time_s = (k + 1) * s.dt_s;
        current_log.actual_state = sim.SimState();
        current_log.reported_state = currentState;
        
        logs.emplace_back(current_log);
    }

    return logs;
}

std::unique_ptr<VehicleInterface> runFaults(std::unique_ptr<VehicleInterface> vehicle, const Scenario& s, const SimClock& clock) {
    if (s.dropout_start_s && s.dropout_end_s) {
        vehicle = std::make_unique<StateDropoutFault>(std::move(vehicle), clock, *s.dropout_start_s, *s.dropout_end_s);
    }
    return vehicle;
}
