#pragma once

#include "Define/RendererApi.h"
#include "Util/NonCopyable.h"

namespace Renderer
{
    class RhiImp : public NonCopyable
    {
    public:
        RhiImp() = default;
        virtual ~RhiImp() = default;

    public:
        virtual RendererApi GetApi() const = 0;
        virtual bool SetUp() = 0;
        virtual void Destroy() = 0;
        virtual void Render() = 0;

    public:
        bool IsSettleUp() const;

    protected:
        bool _setUp = false;
    };
}

