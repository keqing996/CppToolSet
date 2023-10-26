#pragma once

#include "Renderer/RendererApi.h"

namespace Editor
{
    class EditorBackend
    {
    public:
        virtual ~EditorBackend() = default;

    public:
        void SetUp();
        virtual void Destroy() = 0;

    private:
        virtual void SetUpBackend() = 0;

    public:
        static EditorBackend* Create(Renderer::RendererApi api);
    };
}