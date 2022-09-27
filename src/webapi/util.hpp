#pragma once

#include <openmbu_webapi/status.hpp>
#include <string>
#include <nlohmann/json.hpp>

extern bool postJSON(const nlohmann::json &jsonRequest, const std::string &url, nlohmann::json *jsonResponse);
extern OpenMBUWebAPI::Status postJSONAndValidate(const nlohmann::json &jsonRequest, const std::string &url, nlohmann::json *jsonResponse, std::string* statusMsg);
