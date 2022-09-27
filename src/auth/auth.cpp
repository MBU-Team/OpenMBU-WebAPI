#include "auth.hpp"
#include "mbu_auth.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

//void test()
//{
//    CURL* curl;
//    CURLcode res;
//    curl = curl_easy_init();
//    if (curl)
//    {
//        std::string jsonstr = "{\"user_name\": \"test\", \"password\": \"potato\"}";
//
//        //std::string jsonstr = "user_name=test&user_password=potato";
//
//        struct curl_slist *headers = NULL;
//        //headers = curl_slist_append(headers, "Accept: application/json");
//        //headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
//        //headers = curl_slist_append(headers, "Expect:");
//        headers = curl_slist_append(headers, "Content-Type: application/json");
//        //headers = curl_slist_append(headers, "charset: utf-8");
//
//        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openmbu.com/login.php");
//        //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
//        //curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.38.0");
//        //curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//        //const char* data = curl_easy_escape(curl, jsonstr.c_str(), jsonstr.length());
//        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
//        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
//        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//        //curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
//        //curl_easy_setopt(curl, CURLOPT_POST, 1L);
//        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
//        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//        res = curl_easy_perform(curl);
//        if (res != CURLE_OK)
//            std::cout << "Failed to connect to OpenMBU API" << std::endl;
//        curl_easy_cleanup(curl);
//    }
//}

//typedef void (*jsonCallback)(nlohmann::json json);

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

void test()
{
    nlohmann::json login_response {};
    login("tester", "tester", &login_response);
    std::string msg1 = login_response["msg"];
    std::string status1 = login_response["status"];
    std::string displayName1 = "N/A";
    if (login_response.contains("game_display_name"))
        displayName1 = login_response["game_display_name"];
    std::string token1 = "N/A";
    if (login_response.contains("game_display_name"))
        token1 = login_response["game_token"];
    std::cout << "=== LOGIN ===" << std::endl;
    std::cout << "Msg: " << msg1 << std::endl;
    std::cout << "Status: " << status1 << std::endl;
    std::cout << "Display Name: " << displayName1 << std::endl;
    std::cout << "Token: " << token1 << std::endl;

    std::cout << std::endl;

    nlohmann::json session_response {};
    check_session(token1, &session_response);
    std::string msg2 = session_response["msg"];
    std::string status2 = session_response["status"];
    std::string displayName2 = "N/A";
    if (session_response.contains("game_display_name"))
        displayName2 = session_response["game_display_name"];
    std::cout << "=== CHECK SESSION ===" << std::endl;
    std::cout << "Msg: " << msg2 << std::endl;
    std::cout << "Status: " << status2 << std::endl;
    std::cout << "Display Name: " << displayName2 << std::endl;

    std::cout << std::endl;

    nlohmann::json logout_response {};
    logout(token1, &logout_response);
    std::string msg3 = logout_response["msg"];
    std::string status3 = logout_response["status"];
    std::cout << "=== LOGOUT ===" << std::endl;
    std::cout << "Msg: " << msg3 << std::endl;
    std::cout << "Status: " << status3 << std::endl;
}
