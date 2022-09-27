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

enum class MBUAuthStatus
{
    MBU_AUTH_SUCCESS = 0,
    MBU_AUTH_FAILURE,
    MBU_AUTH_ERROR,
    MBU_AUTH_UNKNOWN,
};

class MBUAUTH_EXPORT MBUAccount
{
public:
    MBUAccount();
    ~MBUAccount();

    MBUAuthStatus Login(const std::string& username, const std::string& password, std::string* statusMsg = nullptr);
    MBUAuthStatus CheckSession(std::string* statusMsg = nullptr);
    MBUAuthStatus Logout(std::string* statusMsg = nullptr);

    const std::string& GetUsername() const { return mUsername; }
    const std::string& GetDisplayName() const { return mDisplayName; }
    const std::string& GetGameToken() const { return mGameToken; }

private:
    std::string mUsername;
    std::string mDisplayName;
    std::string mGameToken;
    bool mIsLoggedIn;
};
