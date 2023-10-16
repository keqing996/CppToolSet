#include "IndexBuffer.h"
#include "Application/Application.h"
#include "Define/RendererApi.h"
#include "RendererHardwareInterface/OpenGL/Buffer/IndexBufferOpenGL.h"

namespace Renderer
{
    IndexBuffer* IndexBuffer::Create(const unsigned int* indices, unsigned int length)
    {
        switch (Renderer::GetApi())
        {
            case RendererApi::OpenGL:
                return new IndexBufferOpenGL(indices, length);
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