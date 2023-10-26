#pragma once

namespace Renderer
{
    class PixelShader
    {
    public:
        virtual ~PixelShader() = default;

    public:
        virtual bool Compile() = 0;

    public:
        static PixelShader* Create(const char* shaderContent);
    };
}