#pragma once

#include "RendererApi/RenderHardwareInterface.h"

class RhiD3d11 : public RenderHardwareInterface
{
public:
    bool SetUp() override;
    void Destroy() override;
    void Renderer() override;
};