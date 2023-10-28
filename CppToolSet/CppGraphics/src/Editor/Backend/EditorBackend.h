#pragma once

#include "Define/RendererApi.h"

namespace Editor
{
    class EditorBackend
    {
    public:
        virtual ~EditorBackend() = default;

    public:
        virtual void SetUp() = 0;
        virtual void Destroy() = 0;
        virtual void BeforeUpdate() = 0;
        virtual void AfterUpdate() = 0;

    public:
        static EditorBackend* Create(RendererApi api);
    };
}