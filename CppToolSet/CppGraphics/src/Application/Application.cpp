#include "Define/Math.h"
#include "Application.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

Application::Application()
{
    _pImpl = new ApplicationWinImp();
}

Application::~Application()
{
    DestroyRenderer();
    DestroyWindow();
}

void Application::InitWindow(int windowWidth, int windowHeight)
{
    _width = windowWidth;
    _height = windowHeight;

    _pImpl->RegisterAndCreateWindow(_width, _height, WINDOW_NAME);
    _pImpl->ShowWindow();
}

void Application::DestroyWindow()
{
    _pImpl->DestroyWindow();
}

void Application::SetupRenderer(RendererApi api)
{
    Renderer::Renderer::SetApi(api);

    _pRender = new Renderer::Renderer();

    _pRenderCommand = Renderer::RendererCommand::Create(api);
    _pRenderCommand->SetUp();

    _pEditor = new Editor::Editor(api);
    _pEditor->SetUp();
}

void Application::DestroyRenderer()
{
    if (_pEditor != nullptr)
        _pEditor->Destroy();

    delete _pEditor;

    if (_pRender != nullptr)
        _pRenderCommand->Destroy();

    delete _pRenderCommand;

    delete _pRender;
}

void Application::RunLoop()
{
    while (true)
    {
        MSG msg;
        bool shouldStop = false;
        while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                shouldStop = true;
                break;
            }

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            _pRenderCommand->ClearColor(Vec4{0.2f, 0.2f, 0.2f, 1.0f});

            _pRender->Render(_pRenderCommand);
            _pEditor->Update();

            _pRenderCommand->SwapBuffer();
        }

        if (shouldStop)
            break;
    }
}

#pragma region [Getter]

int Application::GetWindowHeight() const
{
    return _width;
}

int Application::GetWindowWidth() const
{
    return _height;
}

void* Application::GetWindowHandle() const
{
    return reinterpret_cast<void*>(_pImpl->GetWindowHandle());
}

const Input::Keyboard& Application::GetKeyboard() const
{
    return _keyboard;
}

const Input::Mouse& Application::GetMouse() const
{
    return _mouse;
}

const Renderer::Renderer* Application::GetRenderer() const
{
    return _pRender;
}

#pragma endregion

#pragma region [Static Instance]

void Application::CreateInstance()
{
    if (_instance == nullptr)
        _instance = new Application();
}

Application* Application::GetInstance()
{
    return _instance;
}

#pragma endregion


