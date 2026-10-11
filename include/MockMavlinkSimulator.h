#pragma once
#include "SimulationInterface.h"
#include "Simulation.h"
#include <random>
#include <optional>

struct MavlinkState {
    double pos_north_m{};
    double pos_east_m{};
    std::uint16_t heading_cdeg{65535};
    double velocity_north_mps{};
    double velocity_east_mps{};
    std::uint64_t time_since_boot_ms{};
};

struct MavlinkCommand {
    double target_velocity_north_mps{};
    double target_velocity_east_mps{};
};

struct MavlinkParams {
    Limits limits{10.0,1.5,2.5};
    double simulation_noise_m{0.5};
    double telemetry_rate_hz{5.0};
};

class MockMavlinkSimulator : public SimulationInterface {
public:

    // construct a simulated mock Mavlink vehicle with an inital state, sim params and a seed so that the simulation is repeatable
    MockMavlinkSimulator(const VehicleState& initialState, const MavlinkParams& params, std::uint32_t seed);
    
    void command(const MavlinkCommand& command);
    
    // updates from vehicle
    std::optional<MavlinkState> getLatestState() const;

    void update(double dt) override;
    double getTime() const override;

    // true state of the vehicle, without noise
    VehicleState SimState() const override;

private:
    VehicleState state_;
    VehicleCommand command_;
    double time_{0.0};
    MavlinkParams params_;
    std::optional<MavlinkState> latestState_;

    // random number for unique simulation, decided by seed
    std::mt19937 rng_;

    // normal distribution for noise
    std::normal_distribution<double> noise_dist_;

    // time since last telemetry update, used to determine when to send next telemetry update
    double telemetry_time_{0.0};    

};
       