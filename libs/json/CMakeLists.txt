cmake_minimum_required(VERSION 3.23)
project(nlohmann_json)

add_library(nlohmann_json INTERFACE include/nlohmann/json.hpp)
target_include_directories(nlohmann_json INTERFACE include)
