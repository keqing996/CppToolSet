
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
    _pRender = Renderer::Renderer::Create(api);
    _pRender->SetUp();

    _pEditor = new Editor::Editor(api);
    _pEditor->SetUp();
}

void Application::DestroyRenderer()
{
    if (_pEditor != nullptr)
        _pEditor->Destroy();

    delete _pEditor;

    if (_pRender != nullptr)
        _pRender->Destroy();

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

            _pRender->Render();
            _pEditor->Update();

            _pRender->SwapBuffer();
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

Application* Application::_instance = nullptr;

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


