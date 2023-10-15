#include "VertexBuffer.h"
#include "Application/Application.h"
#include "Define/RendererApi.h"
#include "Renderer/RendererHardwareInterface/OpenGL/Buffer/RhiOpenGLVertexBuffer.h"

namespace Renderer
{
    VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int length)
    {
        auto renderApi = Application::GetInstance()->GetRhiRender()->GetApi();
        switch (renderApi)
        {
            case RendererApi::OpenGL:
                return new RhiOpenGLVertexBuffer(vertices, length);
        }

        return nullptr;
    }
}


