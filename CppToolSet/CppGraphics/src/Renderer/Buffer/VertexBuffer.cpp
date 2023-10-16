#include "VertexBuffer.h"
#include "Application/Application.h"
#include "Define/RendererApi.h"
#include "RendererHardwareInterface/OpenGL/Buffer/VertexBufferOpenGL.h"

namespace Renderer
{
    VertexBuffer* VertexBuffer::Create(const float* vertices, unsigned int length)
    {
        switch (Renderer::GetApi())
        {
            case RendererApi::OpenGL:
                return new VertexBufferOpenGL(vertices, length);
            case RendererApi::Vulkan:
                break;
            case RendererApi::D3D11:
                break;
            case RendererApi::D3D12:
                break;
        }

        return nullptr;
    }
}


