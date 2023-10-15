#pragma once

#include "Renderer/Shader/PixelShader.h"

namespace Renderer
{
    class PixelShaderOpenGL : public PixelShader
    {
    public:
        explicit PixelShaderOpenGL(const char* shaderContent);
        ~PixelShaderOpenGL() override;

    public: // override
        bool Compile() override;

    public:
        unsigned int GetShaderId() const;

    private:
        unsigned int _shaderId = 0;
    };
}