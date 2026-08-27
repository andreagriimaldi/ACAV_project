cmake_minimum_required(VERSION 4.0)
project(ACAV)

set(CMAKE_CXX_STANDARD 23)

# Homebrew SDL2 on macOS arm64
set(SDL2_INCLUDE_DIRS "/opt/homebrew/include/SDL2")
set(SDL2_LIBRARIES "/opt/homebrew/lib/libSDL2.dylib")

add_executable(ACAV main.cpp
        Point.cpp
        Point.h
        Point_type.h
        Map.cpp
        Map.h
        Vehicle.cpp
        Vehicle.h
        EgoVehicle.cpp
        EgoVehicle.h
        CPUVehicle.cpp
        CPUVehicle.h
        SDLRenderer.cpp
        SDLRenderer.h
        GlobalPlan.cpp
        GlobalPlan.h
        Perception.cpp
        Perception.h
        IntersectionCoordinator.cpp
        IntersectionCoordinator.h
        Simulator.cpp
        Simulator.h
        MotionPrediction.cpp
        MotionPrediction.h
        AdaptiveCruiseControl.cpp
        AdaptiveCruiseControl.h
        Optimizer.cpp
        Optimizer.h
        EgoTelemetry.cpp
        EgoTelemetry.h
        HUD.cpp
        HUD.h
)

set(CORE_SOURCES
        Map.cpp Vehicle.cpp CPUVehicle.cpp EgoVehicle.cpp
        GlobalPlan.cpp Perception.cpp Point.cpp
        IntersectionCoordinator.cpp
        MotionPrediction.cpp
        MotionPrediction.h
        AdaptiveCruiseControl.cpp
        AdaptiveCruiseControl.h
        Optimizer.cpp
        Optimizer.h
        EgoTelemetry.cpp
        EgoTelemetry.h
        HUD.cpp
        HUD.h)

add_executable(ACAV_mc
        test/mc_main.cpp
        test/MonteCarloSimulator.cpp
        ${CORE_SOURCES})
target_include_directories(ACAV_mc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(ACAV PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)


