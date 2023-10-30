#pragma once

#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "Renderer/RendererCommand.h"

namespace Renderer
{
    struct RhiOpenGLData;

    class RhiOpenGL : public RendererCommand
    {
    public:
        RhiOpenGL();
        ~RhiOpenGL() override;

    public:
        bool SetUp() override;
        void Destroy() override;
        void SwapBuffer() override;
        void ClearColor(Eigen::Vector4f color) override;
        void Submit(VertexArray* pVertArray, ShaderProgram* pShader) override;

    public:
        const RhiOpenGLData* GetData() const;

    private:
        RhiOpenGLData* _pData;

    };
}

