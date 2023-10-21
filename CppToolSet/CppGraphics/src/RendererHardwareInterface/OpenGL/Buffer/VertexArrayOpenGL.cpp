#include "VertexArrayOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "RendererHardwareInterface/OpenGL/Shader/ShaderDataTypeOpenGL.h"

namespace Renderer
{

    VertexArrayOpenGL::VertexArrayOpenGL()
    {
        ::glCreateVertexArrays(1, &_vertexArrayId);
    }

    VertexArrayOpenGL::~VertexArrayOpenGL()
    {
        ::glDeleteVertexArrays(1, &_vertexArrayId);
    }

    void VertexArrayOpenGL::Bind()
    {
        ::glBindVertexArray(_vertexArrayId);
    }

    void VertexArrayOpenGL::UnBind()
    {
        ::glBindVertexArray(0);
    }

    void VertexArrayOpenGL::AddVertexBuffer(const VertexBuffer* pVertexBuffer)
    {
        if (pVertexBuffer == nullptr)
            return;

        ::glBindVertexArray(_vertexArrayId);
        pVertexBuffer->Bind();

        unsigned int index = 0;
        auto vertexBufferLayout = pVertexBuffer->GetLayout();
        for (const auto& element : vertexBufferLayout.GetLayout())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  GetShaderDataCount(element.dataType),
                                  GetShaderDataTypeGlEnum(element.dataType),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  vertexBufferLayout.GetStride(),
                                  (GLvoid*)element.offset);

            index++;
        }

        _vertexBufferVec.push_back(pVertexBuffer);
    }

    void VertexArrayOpenGL::SetIndexBuffer(const IndexBuffer* pIndexBuffer)
    {
        if (pIndexBuffer == nullptr)
            return;

        ::glBindVertexArray(_vertexArrayId);
        pIndexBuffer->Bind();

        _pCurrentIndexBuffer = pIndexBuffer;
    }

    const std::vector<const VertexBuffer*>& VertexArrayOpenGL::GetVertexBufferVector() const
    {
        return _vertexBufferVec;
    }

    const IndexBuffer* VertexArrayOpenGL::GetCurrentIndexBuffer() const
    {
        return _pCurrentIndexBuffer;
    }
}