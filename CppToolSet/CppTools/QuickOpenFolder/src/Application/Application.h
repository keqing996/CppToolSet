#pragma once

#include <Windows.h>
#include "Util/NonCopyable.h"
#include "ImGui/ImGuiRenderer.h"
#include "UiLogic/UiLogic.h"

class Application : public NonCopyable
{
public:
    bool InitWindow(unsigned int windowWidth, unsigned int windowHeight, const wchar_t* name);
    void Destroy();
    void RunLoop();
    int GetWindowHeight() const;
    int GetWindowWidth() const;

private:
    Application() = default;

private:
    static LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsgDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* Specific Msg */
    LRESULT OnMsgWmSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmCloseAndDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    unsigned int _height = 0;
    unsigned int _width = 0;
    WNDCLASSEX* _pWndClassEx = nullptr;
    HINSTANCE _hInst = ::GetModuleHandle(nullptr);
    HWND _hWnd = nullptr;
    ImGuiRenderer* _pImGuiRenderer = nullptr;
    UiLogic* _pUiLogic = nullptr;

private:
    static Application* _instance;
    static constexpr LPCWSTR WND_CLASS_NAME = L"Graphic Renderer";

public:
    static void CreateInstance();
    static Application* GetInstance();
};