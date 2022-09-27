#include "openmbu_webapi/account.hpp"

#include "util.hpp"
#include <nlohmann/json.hpp>

using namespace OpenMBUWebAPI;

bool login(std::string username, std::string password, nlohmann::json *json)
{
    nlohmann::json jsonRequest;
    jsonRequest["user_name"] = username;
    jsonRequest["user_password"] = password;
    return postJSON(jsonRequest, "https://api.openmbu.com/login.php", json);
}

bool check_session(std::string gameToken, nlohmann::json *json)
{
    nlohmann::json jsonRequest;
    jsonRequest["game_token"] = gameToken;
    return postJSON(jsonRequest, "https://api.openmbu.com/check_session.php", json);
}

bool logout(std::string gameToken, nlohmann::json *json)
{
    nlohmann::json jsonRequest;
    jsonRequest["game_token"] = gameToken;
    return postJSON(jsonRequest, "https://api.openmbu.com/logout.php", json);
}

MBUAccount::MBUAccount()
{
    this->mUsername = "";
    this->mDisplayName = "";
    this->mGameToken = "";
    this->mIsLoggedIn = false;
}

MBUAccount::~MBUAccount()
{

}

MBUAuthStatus MBUAccount::Login(const std::string& username, const std::string& password, std::string* statusMsg)
{
    if (this->mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Already logged in";
        return MBUAuthStatus::MBU_AUTH_SUCCESS;
    }

    nlohmann::json login_response {};
    ::login(username, password, &login_response);
    if (!login_response.contains("msg") || !login_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (No Data)";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    MBUAuthStatus ret = MBUAuthStatus::MBU_AUTH_UNKNOWN;

    std::string status = login_response["status"];
    if (status == "success")
        ret = MBUAuthStatus::MBU_AUTH_SUCCESS;
    else if (status == "error")
        ret = MBUAuthStatus::MBU_AUTH_ERROR;
    else if (status == "fail")
        ret = MBUAuthStatus::MBU_AUTH_FAILURE;

    if (login_response.contains("msg"))
        *statusMsg = login_response["msg"];
    else
        *statusMsg = "";

    if (ret == MBUAuthStatus::MBU_AUTH_FAILURE || ret == MBUAuthStatus::MBU_AUTH_ERROR || ret == MBUAuthStatus::MBU_AUTH_UNKNOWN)
        return ret;

    if (!login_response.contains("game_token") || !login_response.contains("game_display_name"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (Missing Information)";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    this->mUsername = username;
    this->mGameToken = login_response["game_token"];
    this->mDisplayName = login_response["game_display_name"];
    this->mIsLoggedIn = true;

    return ret;
}

MBUAuthStatus MBUAccount::CheckSession(std::string *statusMsg)
{
    if (!mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Not logged in";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    nlohmann::json check_session_response {};
    ::check_session(this->mGameToken, &check_session_response);
    if (!check_session_response.contains("msg") || !check_session_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (No Data)";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    MBUAuthStatus ret = MBUAuthStatus::MBU_AUTH_UNKNOWN;

    std::string status = check_session_response["status"];
    if (status == "success")
        ret = MBUAuthStatus::MBU_AUTH_SUCCESS;
    else if (status == "error")
        ret = MBUAuthStatus::MBU_AUTH_ERROR;
    else if (status == "fail")
        ret = MBUAuthStatus::MBU_AUTH_FAILURE;

    if (check_session_response.contains("msg"))
        *statusMsg = check_session_response["msg"];
    else
        *statusMsg = "";

    if (ret == MBUAuthStatus::MBU_AUTH_FAILURE || ret == MBUAuthStatus::MBU_AUTH_ERROR || ret == MBUAuthStatus::MBU_AUTH_UNKNOWN)
    {
        mIsLoggedIn = false;
        return ret;
    }

    if (check_session_response.contains("game_display_name"))
        this->mDisplayName = check_session_response["game_display_name"];

    return ret;
}

MBUAuthStatus MBUAccount::Logout(std::string *statusMsg)
{
    if (!mIsLoggedIn)
    {
        if (statusMsg != nullptr)
            *statusMsg = "Not logged in";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    nlohmann::json logout_response {};
    ::logout(this->mGameToken, &logout_response);
    if (!logout_response.contains("msg") || !logout_response.contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API (No Data)";
        return MBUAuthStatus::MBU_AUTH_ERROR;
    }

    MBUAuthStatus ret = MBUAuthStatus::MBU_AUTH_UNKNOWN;

    std::string status = logout_response["status"];
    if (status == "success")
        ret = MBUAuthStatus::MBU_AUTH_SUCCESS;
    else if (status == "error")
        ret = MBUAuthStatus::MBU_AUTH_ERROR;
    else if (status == "fail")
        ret = MBUAuthStatus::MBU_AUTH_FAILURE;

    if (logout_response.contains("msg"))
        *statusMsg = logout_response["msg"];
    else
        *statusMsg = "";

    mIsLoggedIn = false;

    return ret;
}