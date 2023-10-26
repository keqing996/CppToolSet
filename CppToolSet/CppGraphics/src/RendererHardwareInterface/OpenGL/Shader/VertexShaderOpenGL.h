#pragma once

#include "Renderer/Shader/VertexShader.h"

namespace Renderer
{
    class VertexShaderOpenGL : public VertexShader
    {
    public:
        explicit VertexShaderOpenGL(const char* shaderContent);
        ~VertexShaderOpenGL() override;

    public: // override
        bool Compile() override;

    public:
        unsigned int GetShaderId() const;

    private:
        unsigned int _shaderId = 0;
    };
}