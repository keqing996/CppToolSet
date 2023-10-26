#pragma once

#include "Renderer/RendererApi.h"

namespace Editor
{
    class EditorBackend
    {
    public:
        virtual ~EditorBackend() = default;

    public:
        virtual void SetUp() = 0;
        virtual void Destroy() = 0;

    public:
        static EditorBackend* Create(Renderer::RendererApi api);
    };
}