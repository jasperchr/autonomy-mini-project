cmake_minimum_required(VERSION 3.10)

project("autonomy_miniproject" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -g)

find_package(nlohmann_json 3 REQUIRED)

add_library(vsystem_core src/Simulation.cpp
    src/MockRosSimulator.cpp
    src/MockRosVehicle.cpp
    src/MockMavlinkSimulator.cpp
    src/MockMavlinkVehicle.cpp
    src/Autonomy.cpp
    src/ScenarioRunner.cpp
    src/ScenarioLog.cpp
    src/Evaluator.cpp
    src/Fault.cpp
    src/JsonHandler.cpp
)

target_include_directories(vsystem_core PUBLIC include)

add_executable(vsystem src/main.cpp)
target_link_libraries(vsystem PRIVATE vsystem_core)