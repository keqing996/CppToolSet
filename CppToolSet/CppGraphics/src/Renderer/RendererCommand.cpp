#include "RendererCommand.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{

    RendererCommand* RendererCommand::Create(RendererApi api)
    {
        switch (api)
        {
            case RendererApi::OpenGL:
                return new RhiOpenGL();
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