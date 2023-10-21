#include "IndexBufferOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    IndexBufferOpenGL::IndexBufferOpenGL(const unsigned int* indices, unsigned int length)
    {
        ::glCreateBuffers(1, &_renderId);
        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderId);
        ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        _indicesLength = length;
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

    unsigned int IndexBufferOpenGL::GetIndicesCount() const
    {
        return _indicesLength;
    }
}