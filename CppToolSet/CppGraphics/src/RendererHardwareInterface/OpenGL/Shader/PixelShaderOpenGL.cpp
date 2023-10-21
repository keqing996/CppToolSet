#include "PixelShaderOpenGL.h"
#include "RendererHardwareInterface/OpenGL/GLAD/Glad.h"

namespace Renderer
{

    PixelShaderOpenGL::PixelShaderOpenGL(const char* shaderContent)
    {
        _shaderId = ::glCreateShader(GL_FRAGMENT_SHADER);
        ::glShaderSource(_shaderId, 1, &shaderContent, nullptr);
    }

    PixelShaderOpenGL::~PixelShaderOpenGL()
    {
        ::glDeleteShader(_shaderId);
    }

    bool PixelShaderOpenGL::Compile()
    {
        ::glCompileShader(_shaderId);
        GLint vsSuccess = false;
        ::glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &vsSuccess);
        return vsSuccess;
    }

    unsigned int PixelShaderOpenGL::GetShaderId() const
    {
        return _shaderId;
    }
}