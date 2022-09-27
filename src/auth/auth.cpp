#include "mbu_auth/auth.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool login(std::string username, std::string password, nlohmann::json* json)
{
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        nlohmann::json jsonRequest;
        jsonRequest["user_name"] = username;
        jsonRequest["user_password"] = password;
        std::string jsonstr = jsonRequest.dump();

        std::string readBuffer;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openmbu.com/login.php");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
        {
            nlohmann::json jsonResponse = nlohmann::json::parse(readBuffer);
            *json = jsonResponse;
            //std::cout << jsonResponse.dump(4) << std::endl;
        } else {
            std::cout << "Failed to connect to OpenMBU API" << std::endl;
            return false;
        }

        return true;
    }

    return false;
}

bool check_session(std::string gameToken, nlohmann::json* json)
{
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        nlohmann::json jsonRequest;
        jsonRequest["game_token"] = gameToken;
        std::string jsonstr = jsonRequest.dump();

        std::string readBuffer;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openmbu.com/check_session.php");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
        {
            nlohmann::json jsonResponse = nlohmann::json::parse(readBuffer);
            *json = jsonResponse;
            //std::cout << jsonResponse.dump(4) << std::endl;
        } else {
            std::cout << "Failed to connect to OpenMBU API" << std::endl;
            return false;
        }

        return true;
    }

    return false;
}

bool logout(std::string gameToken, nlohmann::json* json)
{
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        nlohmann::json jsonRequest;
        jsonRequest["game_token"] = gameToken;
        std::string jsonstr = jsonRequest.dump();

        std::string readBuffer;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openmbu.com/logout.php");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
        {
            nlohmann::json jsonResponse = nlohmann::json::parse(readBuffer);
            *json = jsonResponse;
            //std::cout << jsonResponse.dump(4) << std::endl;
        } else {
            std::cout << "Failed to connect to OpenMBU API" << std::endl;
            return false;
        }

        return true;
    }

    return false;
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
