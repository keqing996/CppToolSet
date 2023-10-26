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

    void EditorBackendOpenGL::SetUp()
    {
        auto hWnd = Application::GetInstance()->GetWindowHandle();
        ImGui_ImplWin32_InitForOpenGL(hWnd);
        ImGui_ImplOpenGL3_Init();
    }

    void EditorBackendOpenGL::Destroy()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }

    void EditorBackendOpenGL::BeforeUpdate()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
    }

    void EditorBackendOpenGL::AfterUpdate()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


}