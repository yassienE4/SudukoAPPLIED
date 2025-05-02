cmake_minimum_required(VERSION 3.10)
project(SudukoAPPLIED)

set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Source files
file(GLOB SOURCES
    "*.cpp"
    "data-structures/*.cpp"
)

add_executable(SudukoAPPLIED ${SOURCES})