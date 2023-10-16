#pragma once

#include "Renderer/Buffer/IndexBuffer.h"

namespace Renderer
{
    class IndexBufferOpenGL : public IndexBuffer
    {
    public:
        explicit IndexBufferOpenGL(const unsigned int* indices, unsigned int length);
        ~IndexBufferOpenGL() override;

    private:
        unsigned int _renderId = 0;
    };
}