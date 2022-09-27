#pragma once

#include <string>
#include <nlohmann/json.hpp>

extern bool postJSON(nlohmann::json jsonRequest, const std::string &url, nlohmann::json *jsonResponse);