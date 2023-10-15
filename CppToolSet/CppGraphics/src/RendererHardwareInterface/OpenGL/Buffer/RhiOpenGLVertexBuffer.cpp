#include "RhiOpenGLVertexBuffer.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    RhiOpenGLVertexBuffer::RhiOpenGLVertexBuffer(float* vertices, unsigned int length)
    {
        ::glCreateBuffers(1, &_renderId);
        ::glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STATIC_DRAW);
    }

    RhiOpenGLVertexBuffer::~RhiOpenGLVertexBuffer()
    {
        ::glDeleteBuffers(1, &_renderId);
    }

    void RhiOpenGLVertexBuffer::Bind() const
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, _renderId);
    }

    void RhiOpenGLVertexBuffer::UnBind() const
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}