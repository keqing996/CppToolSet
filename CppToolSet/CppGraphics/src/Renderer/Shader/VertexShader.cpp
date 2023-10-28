#include "VertexShader.h"
#include "Define/RendererApi.h"
#include "Renderer/Renderer.h"
#include "RendererHardwareInterface/OpenGL/Shader/VertexShaderOpenGL.h"

namespace Renderer
{

    VertexShader* VertexShader::Create(const char* shaderContent)
    {
        switch (Renderer::GetApi())
        {
            case RendererApi::OpenGL:
                return new VertexShaderOpenGL(shaderContent);
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