#pragma once
#include "VehicleTypes.h"

class SimulationInterface {
public:
    virtual ~SimulationInterface() = default;

    // update simulation state
    virtual void update(double dt) = 0;

    // get simulation time in seconds
    virtual double getTime() const = 0;

    virtual VehicleState SimState() const = 0;

};