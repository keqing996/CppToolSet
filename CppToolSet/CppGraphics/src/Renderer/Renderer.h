#pragma once

#include "Util/NonCopyable.h"
#include "Define/RendererApi.h"
#include "RendererHardwareInterface/RhiImp.h"

namespace Renderer
{
    class Renderer: public NonCopyable
    {
    public:
        explicit Renderer(RendererApi api);
        ~Renderer();

    public:
        RendererApi GetApi() const;
        void Render();
        void Destroy();

    private:
        RhiImp* _pRhi;
    };
}