#pragma once

#include "RendererHardwareInterface/RhiImp.h"

namespace Renderer
{
    class RhiD3d11 : public RhiImp
    {
    public:
        bool SetUp() override;
        void Destroy() override;
        void Render() override;
    };
}

