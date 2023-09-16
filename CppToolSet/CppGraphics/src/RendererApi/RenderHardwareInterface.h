#pragma once

#include "Define/RendererApi.h"
#include "Util/NonCopyable.h"

class RenderHardwareInterface : public NonCopyable
{
public:
    RenderHardwareInterface() = default;
    virtual ~RenderHardwareInterface() = default;

public:
    virtual bool SetUp() = 0;
    virtual void Destroy() = 0;
    virtual void Renderer() = 0;

public:
    bool IsSettleUp() const;

protected:
    bool _setUp = false;
};