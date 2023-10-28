#include "ShaderProgram.h"
#include "Define/RendererApi.h"
#include "Renderer/Renderer.h"
#include "RendererHardwareInterface/OpenGL/Shader/ShaderProgrmaOpenGL.h"

namespace Renderer
{

    ShaderProgram* ShaderProgram::Create()
    {
        switch (Renderer::GetApi())
        {
            case RendererApi::OpenGL:
                return new ShaderProgramOpenGL();
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