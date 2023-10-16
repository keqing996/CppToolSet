#pragma once

namespace Renderer
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

    public:
        static VertexBuffer* Create(const float* vertices, unsigned int length);
    };
}
