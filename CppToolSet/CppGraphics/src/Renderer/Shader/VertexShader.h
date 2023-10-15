#pragma once

namespace Renderer
{
    class VertexShader
    {
    public:
        virtual ~VertexShader() = default;

    public:
        virtual bool Compile() = 0;

    public:
        static VertexShader* Create(const char* shaderContent);
    };
}