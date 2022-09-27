#include "util.hpp"
#include <curl/curl.h>

using namespace OpenMBUWebAPI;

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

bool postJSON(const nlohmann::json &jsonRequest, const std::string &url, nlohmann::json *jsonResponse)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        std::string jsonstr = jsonRequest.dump();

        std::string readBuffer;

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
        {
            nlohmann::json jsonRes = nlohmann::json::parse(readBuffer);
            *jsonResponse = jsonRes;
        } else
        {
            return false;
        }

        return true;
    }

    return false;
}

OpenMBUWebAPI::Status postJSONAndValidate(const nlohmann::json &jsonRequest, const std::string &url, nlohmann::json *jsonResponse, std::string* statusMsg)
{
    if (!postJSON(jsonRequest, url, jsonResponse))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Failed to connect to server";
        return Status::STATUS_ERROR;
    }

    if (!jsonResponse->contains("msg") || !jsonResponse->contains("status"))
    {
        if (statusMsg != nullptr)
            *statusMsg = "Invalid response from OpenMBU API";
        return Status::STATUS_ERROR;
    }

    Status ret = Status::STATUS_UNKNOWN;

    std::string status = (*jsonResponse)["status"];
    if (status == "success")
        ret = Status::STATUS_SUCCESS;
    else if (status == "error")
        ret = Status::STATUS_ERROR;
    else if (status == "fail")
        ret = Status::STATUS_FAILURE;

    if (statusMsg != nullptr)
    {
        if (jsonResponse->contains("msg"))
            *statusMsg = (*jsonResponse)["msg"];
        else
            *statusMsg = "";
    }

    return ret;
}
