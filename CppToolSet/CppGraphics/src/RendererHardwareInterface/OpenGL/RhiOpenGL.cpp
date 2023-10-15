#include "RhiOpenGL.h"
#include "Application/Application.h"

namespace Renderer
{
    bool RhiOpenGL::SetUp()
    {
        auto pApplication = Application::GetInstance();
        HWND hWnd = pApplication->GetWindowHandle();

        // init openGL pixel format
        PIXELFORMATDESCRIPTOR pfd =
                {
                        sizeof(PIXELFORMATDESCRIPTOR),
                        1,
                        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
                        PFD_TYPE_RGBA,        // The kind of framebuffer.
                        32,                   // Color depth of the framebuffer.
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        24,
                        8,
                        0
                };

        _hDC = ::GetDC(hWnd);
        GLuint pixelFormat = ::ChoosePixelFormat(_hDC, &pfd);
        auto setFormatSuccess = ::SetPixelFormat(_hDC, pixelFormat, &pfd);
        if (!setFormatSuccess)
            return false;

        // create opengl renderer context
        _hRC = ::wglCreateContext(_hDC);
        auto makeContextSuccess = ::wglMakeCurrent(_hDC, _hRC);
        if (!makeContextSuccess)
        {
            ::wglMakeCurrent(nullptr, nullptr);
            ::wglDeleteContext(_hRC);
            return false;
        }

        // init glad
        auto gladSuccess = ::gladLoadGL();
        if (!gladSuccess)
        {
            ::wglMakeCurrent(nullptr, nullptr);
            ::wglDeleteContext(_hRC);
            return false;
        }

        ::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        ::glViewport(0, 0, pApplication->GetWindowHeight(), pApplication->GetWindowWidth());

        return true;
    }

    void RhiOpenGL::Destroy()
    {
        ::wglMakeCurrent(nullptr, nullptr);
        ::wglDeleteContext(_hRC);

        HWND hWnd = Application::GetInstance()->GetWindowHandle();
        ::ReleaseDC(hWnd, _hDC);
    }

    void RhiOpenGL::SwapBuffer()
    {
        ::SwapBuffers(_hDC);
    }

}