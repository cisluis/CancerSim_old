
cmake_minimum_required(VERSION 3.16)

project(CancerSim2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/../include)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )


add_executable(
    ${PROJECT_NAME}_GUI
    GUI.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}_GUI
    ${wxWidgets_LIBRARIES}
    )