#pragma once

#include "BufferLayout.h"

namespace Renderer
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

    public:
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual void SetLayout(BufferLayout&& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

    public:
        static VertexBuffer* Create(const float* vertices, unsigned int length);
    };
}
