#pragma once

#include "Renderer/Buffer/VertexBuffer.h"

namespace Renderer
{
    class VertexBufferOpenGL : public VertexBuffer
    {
    public:
        explicit VertexBufferOpenGL(const float* vertices, unsigned int length);
        ~VertexBufferOpenGL() override;

    public:
        void Bind() const override;
        void UnBind() const override;
        void SetLayout(const BufferLayout& layout) override;
        void SetLayout(BufferLayout&& layout) override;
        const BufferLayout& GetLayout() const override;

    private:
        unsigned int _renderId = 0;
        BufferLayout _bufferLayout;
    };
}