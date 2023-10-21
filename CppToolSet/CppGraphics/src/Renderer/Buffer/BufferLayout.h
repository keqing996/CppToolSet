#pragma once

#include <string>
#include <vector>

#include "Renderer/Shader/ShaderDataType.h"

namespace Renderer
{
    struct BufferElement
    {
        std::string name;
        ShaderDataType dataType;
        unsigned int size;
        unsigned int offset;
        bool normalized;

        explicit BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& inputLayout);

    public:
        const std::vector<BufferElement>& GetLayout() const;
        unsigned int GetStride() const;

    private:
        std::vector<BufferElement> _layout;
        unsigned int _stride;
    };
}