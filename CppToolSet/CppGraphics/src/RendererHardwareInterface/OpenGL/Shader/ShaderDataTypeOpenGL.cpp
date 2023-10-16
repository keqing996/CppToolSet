#include "ShaderDataTypeOpenGL.h"

namespace Renderer
{
    GLenum GetShaderDataTypeGlEnum(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None:
                return GL_FLOAT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return GL_FLOAT;
            case ShaderDataType::Matrix3x3:
            case ShaderDataType::Matrix4x4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
        }

        return GL_FLOAT;
    }
}