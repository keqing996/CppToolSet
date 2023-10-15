#pragma once

namespace Renderer
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer();

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

    public:
        static IndexBuffer* Create(unsigned int* indices, unsigned int length);
    };
}