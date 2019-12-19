
cmake_minimum_required(VERSION 3.16)

project(CancerSim2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/../include)

add_executable(
    ${PROJECT_NAME}
    CLI.cpp
    )
