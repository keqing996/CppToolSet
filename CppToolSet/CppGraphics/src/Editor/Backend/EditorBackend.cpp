#include "EditorBackend.h"

#include "OpenGL/EditorBackendOpenGL.h"

namespace Editor
{

    EditorBackend* EditorBackend::Create(Renderer::RendererApi api)
    {
        switch (api)
        {
            case Renderer::RendererApi::OpenGL:
                return new EditorBackendOpenGL();
            case Renderer::RendererApi::Vulkan:
                break;
            case Renderer::RendererApi::D3D11:
                break;
            case Renderer::RendererApi::D3D12:
                break;
        }

        return nullptr;
    }
}