#pragma once

#include "Util/NonCopyable.h"
#include "Define/RendererApi.h"
#include "Buffer/VertexArray.h"
#include "Shader/ShaderProgram.h"
#include <eigen/Eigen>

namespace Renderer
{
    class RendererCommand : public NonCopyable
    {
    public:
        virtual ~RendererCommand() = default;

    public:
        virtual bool SetUp() = 0;
        virtual void Destroy() = 0;
        virtual void SwapBuffer() = 0;
        virtual void ClearColor(Eigen::Vector4f color) = 0;
        virtual void Submit(VertexArray* pVertArray, ShaderProgram* pShader) = 0;

    public:
        static RendererCommand* Create(RendererApi api);
    };
}