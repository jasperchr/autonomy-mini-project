#pragma once
#include "VehicleTypes.h"
#include <optional>
#include <string>
#include <vector>

struct LogRow {
    double time_s{};
    VehicleState actual_state{};
    std::optional<VehicleState> reported_state;
    VehicleCommand command{};
};

// get vehicle state as a string
std::string stateToString(const VehicleState& state);
std::string stateToString(const std::optional<VehicleState>& state);

// get vehicle command as a string
std::string commandToString(const VehicleCommand& command);

// construct row
std::string writeRow(LogRow row);

// main log function
void writeLog(const std::vector<LogRow>& logs, const std::string& export_path);