#pragma once

#include "Renderer/Buffer/VertexBuffer.h"

namespace Renderer
{
    class RhiOpenGLVertexBuffer : public VertexBuffer
    {
    public:
        explicit RhiOpenGLVertexBuffer(float* vertices, unsigned int length);
        virtual ~RhiOpenGLVertexBuffer();

        void Bind() const override;
        void UnBind() const override;

    private:
        unsigned int _renderId;
    };
}