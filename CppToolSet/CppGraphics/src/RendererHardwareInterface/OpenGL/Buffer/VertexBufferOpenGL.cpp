#include "VertexBufferOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    VertexBufferOpenGL::VertexBufferOpenGL(const float* vertices, unsigned int length)
    {
        ::glCreateBuffers(1, &_renderId);
        ::glBindBuffer(GL_ARRAY_BUFFER, _renderId);
        ::glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertices, GL_STATIC_DRAW);
    }

    VertexBufferOpenGL::~VertexBufferOpenGL()
    {
        ::glDeleteBuffers(1, &_renderId);
    }

    void VertexBufferOpenGL::Bind()
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, _renderId);
    }

    void VertexBufferOpenGL::UnBind()
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferOpenGL::SetLayout(const BufferLayout& layout)
    {
        _bufferLayout = layout;
    }
}