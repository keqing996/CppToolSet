#pragma once

namespace Util
{
    class NonCopyable
    {
    protected:
        NonCopyable() {}
        ~NonCopyable() {}

    private:
        NonCopyable( const NonCopyable& );
        const NonCopyable& operator=( const NonCopyable& );
    };
}


