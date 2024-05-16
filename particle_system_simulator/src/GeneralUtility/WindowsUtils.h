#pragma once

#include <UserEnv.h>
#include <Windows.h>
#include <string>

inline std::string getCurrentUserDirectory()
{
    HANDLE userHandle;
    DWORD pathSize = MAX_PATH;
    char path[MAX_PATH];

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &userHandle))
    {
        throw "Error getting the current user handle";
    }

    if (GetUserProfileDirectoryA(userHandle, path, &pathSize) == FALSE)
    {
        throw "Error getting the current user directory";
    }

    return std::string(path, pathSize - 1);
}