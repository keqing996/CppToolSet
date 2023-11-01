#pragma once

#include <optional>

namespace UI
{
    struct WinDataResource
    {
        void* data;
        unsigned int size;
    };

    class WinResourceLoader
    {
    public:
        WinResourceLoader() = delete;

    public:
        static std::optional<WinDataResource> LoadDataResource(int id);
    };
}