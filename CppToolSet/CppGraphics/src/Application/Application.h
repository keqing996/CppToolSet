#pragma once

#include "Util/NonCopyable.h"
#include "Input/Keyboard/Keyboard.h"
#include "Input/Mouse/Mouse.h"
#include "Define/RendererApi.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"
#include "Editor/Editor.h"
#include "ApplicationWinImp/ApplicationWinImp.h"

class Application : public NonCopyable
{
public:
    friend class ApplicationWinImp;

public:
    ~Application();
    void InitWindow(int windowWidth, int windowHeight);
    void DestroyWindow();
    void SetupRenderer(RendererApi api);
    void DestroyRenderer();
    void RunLoop();

public: // Getter
    int GetWindowHeight() const;
    int GetWindowWidth() const;
    void* GetWindowHandle() const;
    const Input::Keyboard& GetKeyboard() const;
    const Input::Mouse& GetMouse() const;
    const Renderer::Renderer* GetRenderer() const;

private:
    Application();

private:
    /* imp */
    ApplicationWinImp* _pImpl = nullptr;

    /* Basic */
    int _height = 0;
    int _width = 0;

    /* Input */
    Input::Keyboard _keyboard = Input::Keyboard{};
    Input::Mouse _mouse = Input::Mouse{};

    /* Rhi */
    Renderer::RendererCommand* _pRenderCommand = nullptr;

    /* Render */
    Renderer::Renderer* _pRender = nullptr;

    /* Editor */
    Editor::Editor* _pEditor = nullptr;

private:
    inline static Application* _instance = nullptr;
    inline static constexpr const char* WINDOW_NAME = "Graphic Render";

public:
    static void CreateInstance();
    static Application* GetInstance();
};