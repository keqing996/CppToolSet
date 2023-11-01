#pragma once

#include <optional>

namespace UI
{
    struct WinDataResource
    {
        void* data;
        unsigned int size;
    };

    struct WinIconResource
    {
        void* hIcon;
    };

    class WinResourceLoader
    {
    public:
        WinResourceLoader() = delete;

    public:
        static std::optional<WinDataResource> LoadDataResource(int id);
        static std::optional<WinIconResource> LoadIconResource(int id);
    };
}