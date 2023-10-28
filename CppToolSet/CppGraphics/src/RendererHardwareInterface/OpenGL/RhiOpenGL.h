#pragma once

#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "Renderer/Renderer.h"

namespace Renderer
{
    struct RhiOpenGLData;

    class RhiOpenGL : public Renderer
    {
    public:
        RhiOpenGL();
        ~RhiOpenGL() override;

    public:
        bool SetUp() override;
        void Destroy() override;
        void SwapBuffer() override;

    private:
        RhiOpenGLData* _pData;

    };
}

