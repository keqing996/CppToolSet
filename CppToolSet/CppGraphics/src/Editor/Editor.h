#pragma once

#include "Backend/EditorBackend.h"
#include "Renderer/RendererApi.h"
#include "Util/NonCopyable.h"

namespace Editor
{
    class Editor
    {
    public:
        explicit Editor(Renderer::RendererApi api);
        ~Editor() = default;

    public:
        void SetUp();
        void Update();
        void Destroy();

    private:
        EditorBackend* _pBackend = nullptr;

    };
}