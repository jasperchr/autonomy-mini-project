#include "ScenarioRunner.h"
#include "MockRosVehicle.h"
#include "MockMavlinkVehicle.h"
#include "Evaluator.h"
#include "JsonHandler.h"
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

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "provide one json file as an arugment\n";
        return 2;
    };

    Test t;
    t = loadfromJson(argv[1]);

    std::vector<LogRow> rows_a = runMockRos(t.scenario, t.ros_params);
    std::vector<LogRow> rows_b = runMockMavlink(t.scenario, t.mavlink_params);
    

    // run evaluation
    std::vector<Result> evaluation_a = evaluate(rows_a, t.scenario);
    std::cout << '\n' << "MOCK ROS RESULTS" << '\n';
    std::cout << writeResults(evaluation_a) << '\n'; // not sure how is best to output for now

    std::vector<Result> evaluation_b = evaluate(rows_b, t.scenario);
    std::cout << '\n' << "MOCK MAVLINK RESULTS" << '\n';
    std::cout << writeResults(evaluation_b) << '\n'; // not sure how is best to output for now

    // save log
    writeLog(rows_a, "results/"+ t.scenario.name + "_ros.csv");
    writeLog(rows_b, "results/" + t.scenario.name + "_mavlink.csv");


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