#pragma once

#include <Windows.h>
#include "Util/NonCopyable.h"
#include "Input/Keyboard/Keyboard.h"
#include "Input/Mouse/Mouse.h"
#include "Define/RendererApi.h"
#include "Renderer/Renderer.h"

class Application : public NonCopyable
{
public:
    ~Application();
    void InitWindow(int windowWidth, int windowHeight, const wchar_t* name);
    void DestroyWindow();
    void SetupRenderer(RendererApi api);
    void DestroyRenderer();
    void RunLoop();

public: // Gettter
    int GetWindowHeight() const;
    int GetWindowWidth() const;
    HWND GetWindowHandle() const;
    Input::Keyboard::Accessor GetKeyboardAccessor();
    Input::Mouse::Accessor GetMouseAccessor();
    const Renderer::Renderer* GetRenderer() const;
    RendererApi GetRenderApi() const;

private:
    Application() = default;

private:
    static LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsgDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* System */
    LRESULT OnMsgWmClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* Input - Keyboard */
    LRESULT OnMsgWmKeyDownAndSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmKeyUpAndSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /* Input - Mouse */
    LRESULT OnMsgWmMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgWmMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

    /* Basic */
    int _height = 0;
    int _width = 0;
    HINSTANCE _hInst = ::GetModuleHandle(nullptr);
    HWND _hWnd = nullptr;

    /* Input */
    Input::Keyboard _keyboard = Input::Keyboard{};
    Input::Mouse _mouse = Input::Mouse{};

    /* Render */
    Renderer::Renderer* _pRender = nullptr;

private:
    static Application* _instance;
    static constexpr LPCWSTR WND_CLASS_NAME = L"Graphic Render";

public:
    static void CreateInstance();
    static Application* GetInstance();
};