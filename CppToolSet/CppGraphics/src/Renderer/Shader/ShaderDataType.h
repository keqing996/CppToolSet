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

    unsigned int GetShaderDataTypeSize(ShaderDataType type);

    unsigned int GetShaderDataCount(ShaderDataType type);
}