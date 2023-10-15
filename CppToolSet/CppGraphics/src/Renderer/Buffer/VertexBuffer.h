#pragma once

namespace Renderer
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer();

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

    public:
        static VertexBuffer* Create(float* vertices, unsigned int length);
    };
}
