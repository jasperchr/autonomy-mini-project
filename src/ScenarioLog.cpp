#include "ScenarioLog.h"
#include <fstream>
#include <iostream>

std::string stateToString(const VehicleState& state) {
     return std::to_string(state.position.x) + "," + 
            std::to_string(state.position.y) + "," + 
            std::to_string(state.heading_rad) + "," + 
            std::to_string(state.speed_mps);
}

std::string stateToString(const std::optional<VehicleState>& state) {
    if (!state) {
        return ",,,";
    }
    return stateToString(*state);
}

std::string commandToString(const VehicleCommand& command) {
     return std::to_string(command.target_heading_rad) + "," +
            std::to_string(command.target_speed_mps);
}


std::string writeRow(LogRow row) {
     return std::to_string(row.time_s) + "," +
            stateToString(row.actual_state) + "," +
            stateToString(row.reported_state) + "," +
            commandToString(row.command);
            
}



void writeLog(const std::vector<LogRow>& logs, const std::string& export_path) {
    try {
        std::ofstream logFile(export_path);

        for (size_t i{}; i < logs.size(); i++) {
            logFile << writeRow(logs[i]) << '\n';
        }
    }
    catch(...) {
        std::cout << "Failed to open csv for logging";
        return;
    }
    
}