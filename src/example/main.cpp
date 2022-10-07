#include <iostream>
#include <openmbu_webapi/account.hpp>

int main()
{
    std::string statusMsg;
    OpenMBUWebAPI::Account account;
    std::cout << "=== Logging In ===" << std::endl;
    OpenMBUWebAPI::Status status = account.Login("tester", "tester", &statusMsg);
    if (status == OpenMBUWebAPI::Status::STATUS_SUCCESS)
    {
        std::cout << "Logged in successfully!" << std::endl;
        //std::cout << "User Name: " << account.GetUsername() << std::endl;
        std::cout << "Display Name: " << account.GetDisplayName() << std::endl;
        std::cout << "Game Token: " << account.GetGameToken() << std::endl;
        std::cout << "Status Message: " << statusMsg << std::endl;
        std::cout << std::endl;

        std::cout << "=== Checking Session ===" << std::endl;
        status = account.CheckSession(&statusMsg);
        if (status == OpenMBUWebAPI::Status::STATUS_SUCCESS)
        {
            std::cout << "Session is valid!" << std::endl;
            std::cout << "Display Name: " << account.GetDisplayName() << std::endl;
            std::cout << "Status Message: " << statusMsg << std::endl;
            std::cout << std::endl;

            std::cout << "=== Logging Out ===" << std::endl;
            status = account.Logout(&statusMsg);
            if (status == OpenMBUWebAPI::Status::STATUS_SUCCESS)
            {
                std::cout << "Logged out successfully!" << std::endl;
                std::cout << "Status Message: " << statusMsg << std::endl;
            }
            else
            {
                std::cout << "Failed to log out!" << std::endl;
                std::cout << "Status Message: " << statusMsg << std::endl;
            }
        }
        else
        {
            std::cout << "Session is invalid!" << std::endl;
            std::cout << "Status Message: " << statusMsg << std::endl;
        }
    } else {
        std::cout << "Failed to login: " << statusMsg << std::endl;
    }

    return 0;
}
