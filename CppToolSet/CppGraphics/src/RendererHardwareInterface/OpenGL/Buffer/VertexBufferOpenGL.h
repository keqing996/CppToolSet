#pragma once

#include "Renderer/Buffer/VertexBuffer.h"

namespace Renderer
{
    class VertexBufferOpenGL : public VertexBuffer
    {
    public:
        explicit VertexBufferOpenGL(const float* vertices, unsigned int length);
        ~VertexBufferOpenGL() override;

    private:
        unsigned int _renderId = 0;
    };
}