#include "EditorBackendOpenGL.h"
#include "Application/Application.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

namespace Editor
{

    EditorBackendOpenGL::~EditorBackendOpenGL()
    {
    }

    void EditorBackendOpenGL::Destroy()
    {

    }

    void EditorBackendOpenGL::SetUpBackend()
    {
        auto hWnd = Application::GetInstance()->GetWindowHandle();
        //ImGui_ImplWin32_InitForOpenGL(hWnd);
        ImGui_ImplOpenGL3_Init();
    }


}