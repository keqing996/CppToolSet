#pragma once

#include "Renderer/Shader/ShaderDataType.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{
    GLenum GetShaderDataTypeGlEnum(ShaderDataType type);
}