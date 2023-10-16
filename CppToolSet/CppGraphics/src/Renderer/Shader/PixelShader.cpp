#include "PixelShader.h"
#include "Renderer/RendererApi.h"
#include "Renderer/Renderer.h"
#include "RendererHardwareInterface/OpenGL/Shader/PixelShaderOpenGL.h"

namespace Renderer
{

    PixelShader* PixelShader::Create(const char* shaderContent)
    {
        switch (Renderer::GetApi())
        {
            case RendererApi::OpenGL:
                return new PixelShaderOpenGL(shaderContent);
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