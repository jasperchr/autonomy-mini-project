#pragma once
#include <memory>
#include "VehicleInterface.h"
#include "MockRosSimulator.h"

class MockRosVehicle : public VehicleInterface {
public:
    explicit MockRosVehicle(std::unique_ptr<MockRosSimulator> simulator);
    std::optional<VehicleState> getState() const override;
    VehicleHealth getHealth() const override;
    bool sendCommand(const VehicleCommand& command) override;

private:
    std::unique_ptr<MockRosSimulator> simulator_;
};