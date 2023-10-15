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

    constexpr LPCSTR pvsCode =
            "#version 420 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

    constexpr LPCSTR pfsCode =
            "#version 420 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

    constexpr GLfloat Vert[] = {
            -0.5F, -0.5F, +0.0F, // left
            +0.5F, -0.5F, +0.0F, // right
            +0.0F, +0.5F, +0.0F  // top
    };

    void RhiOpenGL::Render()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &pvsCode, nullptr);
        glCompileShader(vertexShader);
        GLint vsSuccess = FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsSuccess);

        // pixel shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &pfsCode, nullptr);
        glCompileShader(fragmentShader);
        GLint fsSuccess = FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsSuccess);

        // shader program
        unsigned int _shader = glCreateProgram();
        glAttachShader(_shader, vertexShader);
        glAttachShader(_shader, fragmentShader);
        glLinkProgram(_shader);
        GLint ProgramSuccess = FALSE;
        glGetProgramiv(_shader, GL_LINK_STATUS, &ProgramSuccess);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vert), Vert, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(_shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glFlush();

        ::SwapBuffers(_hDC);
    }

}