#include "IndexBufferOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    IndexBufferOpenGL::IndexBufferOpenGL(unsigned int* indices, unsigned int length)
    {
        ::glCreateBuffers(1, &_renderId);
        ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, indices, GL_STATIC_DRAW);
    }

    IndexBufferOpenGL::~IndexBufferOpenGL()
    {
        ::glDeleteBuffers(1, &_renderId);
    }

    void IndexBufferOpenGL::Bind() const
    {
        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderId);
    }

    void IndexBufferOpenGL::UnBind() const
    {
        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}