#pragma once
#include <optional>
#include "VehicleTypes.h"

class VehicleInterface {
public:
    virtual ~VehicleInterface() = default;

    // get vehicle state, optional because vehicle may not be connected
    virtual std::optional<VehicleState> getState() const = 0;

    // get vehicle health
    virtual VehicleHealth getHealth() const = 0;

    // send command to vehicle, returns true if command was sent successfully
    virtual bool sendCommand(const VehicleCommand& command) = 0;

};
