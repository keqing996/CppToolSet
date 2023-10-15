#pragma once

#include <Windows.h>

#include "Renderer/RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "Renderer/RendererHardwareInterface//RenderHardwareInterface.h"

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

