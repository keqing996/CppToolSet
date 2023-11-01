#include "WinResourceLoader.h"
#include "Define/WindowsPlatform.h"

namespace UI
{
    std::optional<WinDataResource> WinResourceLoader::LoadDataResource(int id)
    {
        HINSTANCE hInst = GetModuleHandle(nullptr);

        HRSRC findRes = ::FindResource(hInst, MAKEINTRESOURCE(id), RT_RCDATA);
        if (findRes)
        {
            HGLOBAL mem = ::LoadResource(hInst, findRes);
            if (mem)
            {
                DWORD size = SizeofResource(hInst, findRes);
                void* data = LockResource(mem);

                if (data)
                {
                    WinDataResource result { data, static_cast<unsigned int>(size) };
                    return result;
                }
                else
                    return std::nullopt;
            }
            else
                return std::nullopt;
        }
        else
            return std::nullopt;
    }

    std::optional<WinIconResource> WinResourceLoader::LoadIconResource(int id)
    {
        auto hIcon = ::LoadIconW(GetModuleHandle(nullptr), MAKEINTRESOURCE(id));
        if (hIcon == nullptr)
            return std::nullopt;

        WinIconResource result { hIcon };
        return result;
    }
}