#pragma once

namespace Util
{
    class NonConstructible
    {
    public:
        NonConstructible() = delete;
        ~NonConstructible() = delete;
    };
}