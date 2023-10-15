#pragma once

#include "Define/RendererApi.h"
#include "Util/NonCopyable.h"

namespace Renderer
{
    class RenderHardwareInterface : public NonCopyable
    {
    public:
        RenderHardwareInterface() = default;
        virtual ~RenderHardwareInterface() = default;

    public:
        virtual RendererApi GetApi() const = 0;
        virtual bool SetUp() = 0;
        virtual void Destroy() = 0;
        virtual void Renderer() = 0;

    public:
        bool IsSettleUp() const;

    protected:
        bool _setUp = false;
    };
}

