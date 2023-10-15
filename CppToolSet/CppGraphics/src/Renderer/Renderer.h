#pragma once

#include "Util/NonCopyable.h"
#include "Define/RendererApi.h"

namespace Renderer
{
    class Renderer: public NonCopyable
    {
    public:
        virtual ~Renderer() = default;

    public: // static
        static RendererApi GetApi();
        static Renderer* Create(RendererApi api);

    public: // virtual
        virtual bool SetUp() = 0;
        virtual void Destroy() = 0;
        virtual void SwapBuffer() = 0;

    public:
        void Render();

    private:
        inline static RendererApi _api;
    };
}