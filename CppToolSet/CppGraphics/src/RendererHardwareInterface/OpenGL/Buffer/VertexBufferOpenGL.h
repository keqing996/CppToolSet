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
        void Bind() override;
        void UnBind() override;
        void SetLayout(const BufferLayout& layout) override;

    private:
        unsigned int _renderId = 0;
        BufferLayout _bufferLayout;
    };
}