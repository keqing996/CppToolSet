#pragma once

#include "Renderer/RendererHardwareInterface//RenderHardwareInterface.h"

namespace Renderer
{
    class RhiD3d11 : public RenderHardwareInterface
    {
    public:
        bool SetUp() override;
        void Destroy() override;
        void Renderer() override;
    };
}

