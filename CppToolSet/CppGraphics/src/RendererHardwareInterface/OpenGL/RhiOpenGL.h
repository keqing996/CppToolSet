#pragma once

#include <Windows.h>

#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "Renderer/Renderer.h"

namespace Renderer
{
    class RhiOpenGL : public Renderer
    {
    public:
        ~RhiOpenGL() override = default;

    public:
        bool SetUp() override;
        void Destroy() override;
        void SwapBuffer() override;

    private:
        HDC _hDC;
        HGLRC _hRC;
    };
}

