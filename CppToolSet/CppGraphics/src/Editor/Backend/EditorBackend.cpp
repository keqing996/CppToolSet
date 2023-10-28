#include "EditorBackend.h"
#include "Application/Application.h"
#include "Define/RendererApi.h"
#include "OpenGL/EditorBackendOpenGL.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

namespace Editor
{
    EditorBackend* EditorBackend::Create(RendererApi api)
    {
        switch (api)
        {
            case RendererApi::OpenGL:
                return new EditorBackendOpenGL();
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