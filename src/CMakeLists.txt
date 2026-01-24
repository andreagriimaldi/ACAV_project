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
)

target_include_directories(ACAV PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(ACAV ${SDL2_LIBRARIES})



