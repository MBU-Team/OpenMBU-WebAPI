cmake_minimum_required(VERSION 3.23)
project(OpenMBUWebAPI)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/json)

if (NOT DEFINED ProjectOutDir)
    set(ProjectOutDir "${CMAKE_SOURCE_DIR}/run")
endif()

add_subdirectory(src)