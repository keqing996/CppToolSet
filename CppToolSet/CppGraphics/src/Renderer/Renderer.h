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
        inline static RendererApi GetApi();
        void Render();
        void Destroy();

    private:
        static RendererApi _api;
        RhiImp* _pRhi;
    };
}