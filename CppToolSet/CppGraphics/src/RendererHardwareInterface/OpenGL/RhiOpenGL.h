#pragma once

#include <Windows.h>

#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"
#include "RendererHardwareInterface/RhiImp.h"

namespace Renderer
{
    class RhiOpenGL : public RhiImp
    {
    public:
        RendererApi GetApi() const override;
        bool SetUp() override;
        void Destroy() override;
        void Render() override;

    private:
        HDC _hDC;
        HGLRC _hRC;
    };
}

