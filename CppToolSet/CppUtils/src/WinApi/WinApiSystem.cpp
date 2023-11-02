
#include "../../include/WinApi/WindowsPlatform.h"
#include "../../include/WinApi/WinApiSystem.h"

namespace WinApi::System
{
    std::wstring GetWindowsUserName()
    {
        wchar_t userNameBuffer[255];
        DWORD userNameBufferLen;
        ::GetUserName(userNameBuffer, &userNameBufferLen);
        return { userNameBuffer };
    }

    bool CreateDependentProcess(const std::wstring& cmdLine)
    {
        STARTUPINFO si;
        ::ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ::ZeroMemory( &pi, sizeof(pi) );

        // Start the child process.
        if( !::CreateProcessW(
                nullptr,                                // No module name (use command line)
                const_cast<LPWSTR>(cmdLine.c_str()),        // Command line
                nullptr,                                // Process handle not inheritable
                nullptr,                                // Thread handle not inheritable
                FALSE,                                  // Set handle inheritance to FALSE
                0,                                      // No creation flags
                nullptr,                                // Use parent's environment block
                nullptr,                                // Use parent's starting directory
                &si,                                    // Pointer to STARTUPINFO structure
                &pi )                                   // Pointer to PROCESS_INFORMATION structure
                )
        {
            return false;
        }

        // Wait until child process exits.
        ::WaitForSingleObject( pi.hProcess, INFINITE );

        // Close process and thread handles.
        ::CloseHandle( pi.hProcess );
        ::CloseHandle( pi.hThread );

        return true;
    }
}
