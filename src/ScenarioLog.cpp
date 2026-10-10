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

        // add column names to logfile
        logFile << "t_s,"
           "actual_x_m,actual_y_m,actual_heading_rad,actual_speed_mps,"
           "reported_x_m,reported_y_m,reported_heading_rad,reported_speed_mps,"
           "cmd_heading_rad,cmd_speed_mps\n";

        for (size_t i{}; i < logs.size(); i++) {
            logFile << writeRow(logs[i]) << '\n';
        }
    }
    catch(...) {
        std::cout << "Failed to open csv for logging";
        return;
    }
    
}