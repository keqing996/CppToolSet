#pragma once

#include "Util/NonCopyable.h"
#include "Define/RendererApi.h"

namespace Renderer
{
    class Renderer: public NonCopyable
    {
    public: // static
        static RendererApi SetApi(RendererApi api);
        static RendererApi GetApi();

    public:
        void Render();

    private:
        inline static RendererApi _api;
    };
}