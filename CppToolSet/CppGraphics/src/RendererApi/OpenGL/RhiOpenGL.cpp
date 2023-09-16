
#include "RhiOpenGL.h"
#include "Application/Application.h"

bool RhiOpenGL::SetUp()
{
    HWND hWnd = Application::GetInstance()->GetWindowHandle();

    // init openGL context
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
    ::SetPixelFormat(_hDC, pixelFormat, &pfd);

    _hRC = ::wglCreateContext(_hDC);
    ::wglMakeCurrent(_hDC, _hRC);

    ::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    ::glViewport(0, 0, 800, 600);

    _setUp = true;

    return true;
}

void RhiOpenGL::Destroy()
{
    if (!_setUp)
        return;

    ::wglMakeCurrent(nullptr, nullptr);
    ::wglDeleteContext(_hRC);

    HWND hWnd = Application::GetInstance()->GetWindowHandle();
    ::ReleaseDC(hWnd, _hDC);
}

void RhiOpenGL::Renderer()
{
    ::glClear(GL_COLOR_BUFFER_BIT);

    ::glBegin(GL_TRIANGLES);
    ::glColor3f(1.0f, 0.0f, 0.0f);
    ::glVertex2f(-0.5f, -0.5f);
    ::glColor3f(0.0f, 1.0f, 0.0f);
    ::glVertex2f(0.5f, -0.5f);
    ::glColor3f(0.0f, 0.0f, 1.0f);
    ::glVertex2f(0.0f, 0.5f);
    ::glEnd();

    ::SwapBuffers(_hDC);
}
