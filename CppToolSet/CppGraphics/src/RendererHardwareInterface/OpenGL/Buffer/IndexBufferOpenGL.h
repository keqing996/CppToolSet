#pragma once

#include "Renderer/Buffer/IndexBuffer.h"

namespace Renderer
{
    class IndexBufferOpenGL : public IndexBuffer
    {
    public:
        explicit IndexBufferOpenGL(unsigned int* indices, unsigned int length);
        ~IndexBufferOpenGL() override;

        void Bind() const override;
        void UnBind() const override;

    private:
        unsigned int _renderId = 0;
    };
}