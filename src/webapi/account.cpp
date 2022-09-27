#include "openmbu_webapi/account.hpp"

#include "util.hpp"
#include <nlohmann/json.hpp>

using namespace OpenMBUWebAPI;

Account::Account()
{
    this->mUsername = "";
    this->mDisplayName = "";
    this->mGameToken = "";
    this->mIsLoggedIn = false;
}

Account::~Account() = default;

Status Account::Login(const std::string& username, const std::string& password, std::string* statusMsg)
{
    if (this->mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Already logged in";
        return Status::STATUS_SUCCESS;
    }

    nlohmann::json login_response {};

    nlohmann::json jsonRequest;
    jsonRequest["user_name"] = username;
    jsonRequest["user_password"] = password;
    if (!postJSON(jsonRequest, "https://api.openmbu.com/login.php", &login_response))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Failed to connect to server";
        return Status::STATUS_ERROR;
    }

    if (!login_response.contains("msg") || !login_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API";
        return Status::STATUS_ERROR;
    }

    Status ret = Status::STATUS_UNKNOWN;

    std::string status = login_response["status"];
    if (status == "success")
        ret = Status::STATUS_SUCCESS;
    else if (status == "error")
        ret = Status::STATUS_ERROR;
    else if (status == "fail")
        ret = Status::STATUS_FAILURE;

    if (statusMsg != nullptr)
    {
        if (login_response.contains("msg"))
            *statusMsg = login_response["msg"];
        else
            *statusMsg = "";
    }

    if (ret == Status::STATUS_FAILURE || ret == Status::STATUS_ERROR || ret == Status::STATUS_UNKNOWN)
        return ret;

    if (!login_response.contains("game_token") || !login_response.contains("game_display_name"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid login response from OpenMBU API";
        return Status::STATUS_ERROR;
    }

    this->mUsername = username;
    this->mGameToken = login_response["game_token"];
    this->mDisplayName = login_response["game_display_name"];
    this->mIsLoggedIn = true;

    return ret;
}

Status Account::CheckSession(std::string *statusMsg)
{
    if (!mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Not logged in";
        return Status::STATUS_ERROR;
    }

    nlohmann::json check_session_response {};
    nlohmann::json jsonRequest;
    jsonRequest["game_token"] = this->mGameToken;
    if(!postJSON(jsonRequest, "https://api.openmbu.com/check_session.php", &check_session_response))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Failed to connect to server";
        return Status::STATUS_ERROR;
    }

    if (!check_session_response.contains("msg") || !check_session_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (No Data)";
        return Status::STATUS_ERROR;
    }

    Status ret = Status::STATUS_UNKNOWN;

    std::string status = check_session_response["status"];
    if (status == "success")
        ret = Status::STATUS_SUCCESS;
    else if (status == "error")
        ret = Status::STATUS_ERROR;
    else if (status == "fail")
        ret = Status::STATUS_FAILURE;

    if (statusMsg != nullptr)
    {
        if (check_session_response.contains("msg"))
            *statusMsg = check_session_response["msg"];
        else
            *statusMsg = "";
    }

    if (ret == Status::STATUS_FAILURE || ret == Status::STATUS_ERROR || ret == Status::STATUS_UNKNOWN)
    {
        mIsLoggedIn = false;
        return ret;
    }

    if (check_session_response.contains("game_display_name"))
        this->mDisplayName = check_session_response["game_display_name"];

    return ret;
}

Status Account::Logout(std::string *statusMsg)
{
    if (!mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Not logged in";
        return Status::STATUS_ERROR;
    }

    nlohmann::json logout_response {};
    nlohmann::json jsonRequest;
    jsonRequest["game_token"] = this->mGameToken;
    if(!postJSON(jsonRequest, "https://api.openmbu.com/logout.php", &logout_response))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Failed to connect to server";
        return Status::STATUS_ERROR;
    }

    if (!logout_response.contains("msg") || !logout_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (No Data)";
        return Status::STATUS_ERROR;
    }

    Status ret = Status::STATUS_UNKNOWN;

    std::string status = logout_response["status"];
    if (status == "success")
        ret = Status::STATUS_SUCCESS;
    else if (status == "error")
        ret = Status::STATUS_ERROR;
    else if (status == "fail")
        ret = Status::STATUS_FAILURE;

    if (statusMsg != nullptr)
    {
        if (logout_response.contains("msg"))
            *statusMsg = logout_response["msg"];
        else
            *statusMsg = "";
    }

    mIsLoggedIn = false;

    return ret;
}
