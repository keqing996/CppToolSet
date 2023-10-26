#include "Editor.h"

namespace Editor
{

    Editor::Editor(Renderer::RendererApi api)
    {
        _pBackend = EditorBackend::Create(api);
    }

    void Editor::SetUp()
    {

    }

    void Editor::Destroy()
    {

    }
}