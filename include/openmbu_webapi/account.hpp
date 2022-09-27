#pragma once

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
    enum class Status
    {
        STATUS_SUCCESS = 0,
        STATUS_FAILURE,
        STATUS_ERROR,
        STATUS_UNKNOWN,
    };

    class MBUAUTH_EXPORT Account
    {
    public:
        Account();
        ~Account();

        Status Login(const std::string &username, const std::string &password, std::string *statusMsg = nullptr);
        Status CheckSession(std::string *statusMsg = nullptr);
        Status Logout(std::string *statusMsg = nullptr);

        const std::string &GetUsername() const { return mUsername; }
        const std::string &GetDisplayName() const { return mDisplayName; }
        const std::string &GetGameToken() const { return mGameToken; }
        const bool &IsLoggedIn() const { return mIsLoggedIn; }

    private:
        std::string mUsername;
        std::string mDisplayName;
        std::string mGameToken;
        bool mIsLoggedIn;
    };
}
