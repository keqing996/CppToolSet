#pragma once

namespace Renderer
{
    enum class ShaderDataType: int
    {
        None,
        Bool,
        Float,
        Float2,
        Float3,
        Float4,
        Matrix3x3,
        Matrix4x4,
        Int,
        Int2,
        Int3,
        Int4
    };

    static unsigned int GetShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None:
                return 0;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;
            case ShaderDataType::Matrix3x3:
                return 4 * 3 * 3;
            case ShaderDataType::Matrix4x4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;
        }

        return 0;
    }
}