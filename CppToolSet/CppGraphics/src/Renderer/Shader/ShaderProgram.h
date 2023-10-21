#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

namespace Renderer
{
    class ShaderProgram
    {
    public:
        virtual ~ShaderProgram() = default;

    public:
        virtual void AddVertexShader(VertexShader* pVertexShader) = 0;
        virtual void AddPixelShader(PixelShader* pPixelShader) = 0;
        virtual bool Link() = 0;
        virtual void Bind() = 0;

    public:
        static ShaderProgram* Create();
    };
}