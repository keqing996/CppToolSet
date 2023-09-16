#pragma once

#include "OpenGLInclude.h"
#include "RendererApi/RenderHardwareInterface.h"

class RhiOpenGL : public RenderHardwareInterface
{
public:
    bool SetUp() override;
    void Destroy() override;
    void Renderer() override;

private:
    HDC _hDC;
    HGLRC _hRC;
};