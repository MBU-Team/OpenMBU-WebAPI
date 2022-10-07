#pragma once

#include <openmbu_webapi/status.hpp>

#include <string>

#ifdef MBUAUTH_DLL
#ifdef MBUAUTH_BUILD
#define MBUAUTH_EXPORT __declspec(dllexport)
#else
#define MBUAUTH_EXPORT __declspec(dllimport)
#endif
#else
#define MBUAUTH_EXPORT
#endif

namespace OpenMBUWebAPI
{

    class MBUAUTH_EXPORT Account
    {
    public:
        Account();
        ~Account();

        Status Login(const std::string &username, const std::string &password, std::string *statusMsg = nullptr);
        Status LoginSession(const std::string &token, std::string *statusMsg = nullptr);
        Status CheckSession(std::string *statusMsg = nullptr);
        Status Logout(std::string *statusMsg = nullptr);

        const std::string &GetDisplayName() const { return mDisplayName; }
        const std::string &GetGameToken() const { return mGameToken; }
        const bool &IsLoggedIn() const { return mIsLoggedIn; }

    private:
        std::string mDisplayName;
        std::string mGameToken;
        bool mIsLoggedIn;
    };
}
