#pragma once
#include <memory>
#include "VehicleInterface.h"
#include "MockMavlinkSimulator.h"

class MockMavlinkVehicle : public VehicleInterface {
public:
    explicit MockMavlinkVehicle(std::unique_ptr<MockMavlinkSimulator> simulator);
    std::optional<VehicleState> getState() const override;
    VehicleHealth getHealth() const override;
    bool sendCommand(const VehicleCommand& command) override;

private:
    std::unique_ptr<MockMavlinkSimulator> simulator_;
};

VehicleState toVehicleState (const MavlinkState& mav_state);
MavlinkCommand toMockMavCommand (const VehicleCommand& command);