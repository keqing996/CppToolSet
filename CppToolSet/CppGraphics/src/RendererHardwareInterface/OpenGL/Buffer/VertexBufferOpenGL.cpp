#include "VertexBufferOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    VertexBufferOpenGL::VertexBufferOpenGL(float* vertices, unsigned int length)
    {
        ::glCreateBuffers(1, &_renderId);
        ::glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STATIC_DRAW);
    }

    VertexBufferOpenGL::~VertexBufferOpenGL()
    {
        ::glDeleteBuffers(1, &_renderId);
    }

    void VertexBufferOpenGL::Bind() const
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, _renderId);
    }

    void VertexBufferOpenGL::UnBind() const
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}