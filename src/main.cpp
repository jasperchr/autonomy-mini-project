#include "ScenarioRunner.h"
#include "MockRosVehicle.h"
#include "MockMavlinkVehicle.h"
#include "Evaluator.h"
#include <iostream>

static std::vector<LogRow> runMockRos(const Scenario& s, const SimulationParams& params) {
    SimClock clock;

    auto simulator = std::make_unique<MockRosSimulator>(s.initial_state, params, s.seed);
    SimulationInterface* sim = simulator.get();
    std::unique_ptr<VehicleInterface> vehicle = std::make_unique<MockRosVehicle>(std::move(simulator));
    vehicle = runFaults(std::move(vehicle), s, clock);

    return run(s, *vehicle, *sim, clock);

}

static std::vector<LogRow> runMockMavlink(const Scenario& s, const SimulationParams& params) {
    
    SimClock clock;

    auto simulator = std::make_unique<MockMavlinkSimulator>(s.initial_state, params, s.seed);
    SimulationInterface* sim = simulator.get();
    std::unique_ptr<VehicleInterface> vehicle = std::make_unique<MockMavlinkVehicle>(std::move(simulator));
    vehicle = runFaults(std::move(vehicle), s, clock);

    return run(s, *vehicle, *sim, clock);

}

int main() {

    // initialize scenario
    Scenario s;
    s.seed = 5;

    // add tests to evaluate
    s.speed_limit_mps = 5.1;
    s.waypoint_tolerance_m = 3.3;
    s.geofence = {-10,160,-10,10};

    // add fault
    s.dropout_start_s = 36.0;
    s.dropout_end_s = 48.0;


    // set params for simulation
    SimulationParams params;
    params.limits = {10.0, 1.0, 5.0};
    params.simulation_noise_m = 0.5;
    params.telemetry_rate_hz = 10;

    SimulationParams mav_params;
    mav_params.limits = {10.0, 1.5, 2.5};
    mav_params.simulation_noise_m = 0.5;
    mav_params.telemetry_rate_hz = 5.0;

    std::vector<LogRow> rows_a = runMockRos(s, params);
    std::vector<LogRow> rows_b = runMockMavlink(s, mav_params);
    

    // run evaluation
    std::vector<Result> evaluation_a = evaluate(rows_a, s);
    std::cout << '\n' << "MOCK ROS RESULTS" << '\n';
    std::cout << writeResults(evaluation_a) << '\n'; // not sure how is best to output for now

    std::vector<Result> evaluation_b = evaluate(rows_b, s);
    std::cout << '\n' << "MOCK MAVLINK RESULTS" << '\n';
    std::cout << writeResults(evaluation_b) << '\n'; // not sure how is best to output for now

    // save log
    writeLog(rows_a, "results/mock_ros_test.csv");
    writeLog(rows_b, "results/mock_mavlink_test.csv");


    // return 0 for pass, 1 for fail
    for (const Result& r : evaluation_a) {
        if (r.verdict != Verdict::PASS) {
            return 1;
        }
    }
    for (const Result& r : evaluation_b) {
        if (r.verdict != Verdict::PASS) {
            return 1;
        }
    }

    return 0;
}