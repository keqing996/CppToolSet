#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Renderer
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void AddVertexBuffer(const VertexBuffer* pVertexBuffer) = 0;
        virtual void SetIndexBuffer(const IndexBuffer* pIndexBuffer) = 0;
        virtual const std::vector<const VertexBuffer*>& GetVertexBufferVector() const = 0;
        virtual const IndexBuffer* GetCurrentIndexBuffer() const = 0;

    public:
        static VertexArray* Create();
    };
}