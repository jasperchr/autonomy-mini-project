#include "ScenarioRunner.h"
#include "MockRosVehicle.h"
#include "Evaluator.h"
#include <iostream>


int main() {

    // initialize scenario
    Scenario s;

    // add tests to evaluate
    s.speed_limit_mps = 5.1;
    s.waypoint_tolerance_m = 3.3;

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

    // run evaluation
    std::vector<Result> evaluation = evaluate(rows, s);
    std::cout << writeResults(evaluation) << '\n'; // not sure how is best to output for now

    // save log
    writeLog(rows, "results/test.csv");

    // return 1 for pass, 0 for fail
    for (const Result& r : evaluation) {
        if (r.verdict != Verdict::PASS) {
            return 1;
        }
    }

    return 0;
}