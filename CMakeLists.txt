cmake_minimum_required(VERSION 3.23)
project(OpenMBUWebAPI)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/json)

set(ProjectOutDir "${CMAKE_SOURCE_DIR}/run")

add_subdirectory(src)