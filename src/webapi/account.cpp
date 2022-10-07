#include "openmbu_webapi/account.hpp"

#include "util.hpp"
#include <nlohmann/json.hpp>

using namespace OpenMBUWebAPI;

Account::Account()
{
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
    Status ret = postJSONAndValidate(jsonRequest, "https://api.openmbu.com/login.php", &login_response, statusMsg);
    if (ret != Status::STATUS_SUCCESS)
        return ret;

    if (!login_response.contains("game_token") || !login_response.contains("game_display_name"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid login response from OpenMBU API";
        return Status::STATUS_ERROR;
    }

    this->mGameToken = login_response["game_token"];
    this->mDisplayName = login_response["game_display_name"];
    this->mIsLoggedIn = true;

    return ret;
}

Status Account::LoginSession(const std::string& token, std::string* statusMsg)
{
    if (this->mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Already logged in";
        return Status::STATUS_SUCCESS;
    }

    nlohmann::json check_session_response {};
    nlohmann::json jsonRequest;
    jsonRequest["game_token"] = token;
    Status ret = postJSONAndValidate(jsonRequest, "https://api.openmbu.com/check_session.php", &check_session_response, statusMsg);
    if (ret != Status::STATUS_SUCCESS)
        return ret;

    if (check_session_response.contains("game_display_name"))
        this->mDisplayName = check_session_response["game_display_name"];

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
    Status ret = postJSONAndValidate(jsonRequest, "https://api.openmbu.com/check_session.php", &check_session_response, statusMsg);
    if (ret != Status::STATUS_SUCCESS)
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
    Status ret = postJSONAndValidate(jsonRequest, "https://api.openmbu.com/logout.php", &logout_response, statusMsg);
    //if (ret != Status::STATUS_SUCCESS)
    //    return ret;

    mIsLoggedIn = false;

    return ret;
}
