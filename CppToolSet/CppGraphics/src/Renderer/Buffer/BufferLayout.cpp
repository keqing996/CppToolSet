#include "BufferLayout.h"

namespace Renderer
{

    BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
            : name(name)
            , dataType(type)
            , size(GetShaderDataTypeSize(type))
            , offset(0)
            , normalized(normalized)
    {
    }

    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& inputLayout)
        : _layout(inputLayout)
        , _stride(0)
    {
        unsigned int offset = 0;
        for (auto& eachLayout : _layout)
        {
            eachLayout.offset = offset;
            offset += eachLayout.size;
            _stride += eachLayout.size;
        }
    }

    const std::vector<BufferElement>& BufferLayout::GetLayout() const
    {
        return _layout;
    }

    unsigned int BufferLayout::GetStride() const
    {
        return _stride;
    }
}