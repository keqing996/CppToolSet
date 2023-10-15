#pragma once

#include <Windows.h>

#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "RendererHardwareInterface/RenderHardwareInterface.h"

namespace Renderer
{
    class RhiOpenGL : public RenderHardwareInterface
    {
    public:
        RendererApi GetApi() const override;
        bool SetUp() override;
        void Destroy() override;
        void Renderer() override;

    private:
        HDC _hDC;
        HGLRC _hRC;
    };
}

