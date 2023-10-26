#pragma once

#include "Renderer/Buffer/IndexBuffer.h"

namespace Renderer
{
    class IndexBufferOpenGL : public IndexBuffer
    {
    public:
        explicit IndexBufferOpenGL(const unsigned int* indices, unsigned int length);
        ~IndexBufferOpenGL() override;

    public:
        void Bind() const override;
        void UnBind() const override;
        unsigned int GetIndicesCount() const override;

    private:
        unsigned int _indicesLength = 0;
        unsigned int _renderId = 0;
    };
}