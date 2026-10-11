#pragma once
#include "SimulationInterface.h"
#include "Simulation.h"
#include <random>
#include <optional>

class MockRosSimulator : public SimulationInterface {
public:

    // construct a simulated mock ROS vehicle with an inital state, sim params and a seed so that the simulation is repeatable
    MockRosSimulator(const VehicleState& initialState, const SimulationParams& params, std::uint32_t seed);
    
    void command(const VehicleCommand& command);
    
    // updates from vehicle
    std::optional<VehicleState> getLatestState() const;

    void update(double dt) override;
    double getTime() const override;

    // true state of the vehicle, without noise
    VehicleState SimState() const override;

private:
    VehicleState state_;
    VehicleCommand command_;
    double time_{0.0};
    SimulationParams params_;
    std::optional<VehicleState> latestState_;

    // random number for unique simulation, decided by seed
    std::mt19937 rng_;

    // bias for gaussian error
    Vec2 position_bias_{};

    // time since last telemetry update, used to determine when to send next telemetry update
    double telemetry_time_{0.0};    

};
       