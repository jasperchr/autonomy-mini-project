#include "ScenarioRunner.h"
#include "MockRosVehicle.h"


int main() {

    // initialize scenario
    Scenario s;

    // set params for simulation
    SimulationParams params;
    params.limits = {10.0, 1.0, 5.0};
    params.simulation_noise_m = 0.5;
    params.telemetry_rate_hz = 10;

    // build simulated vehicle
    auto simulator = std::make_unique<MockRosSimulator>(s.initial_state, params, s.seed);
    SimulationInterface* sim = simulator.get();
    MockRosVehicle vehicle(std::move(simulator));

    // run scenario
    std::vector<LogRow> rows = run(s, vehicle, *sim);

    // save log
    writeLog(rows, "results/test.csv");

    return 0;
}