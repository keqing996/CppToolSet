#pragma once

namespace Renderer
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

    public:
        static IndexBuffer* Create(const unsigned int* indices, unsigned int length);
    };
}