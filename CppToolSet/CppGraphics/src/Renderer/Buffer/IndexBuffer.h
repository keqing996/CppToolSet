#pragma once

namespace Renderer
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

    public:
        static IndexBuffer* Create(const unsigned int* indices, unsigned int length);
    };
}