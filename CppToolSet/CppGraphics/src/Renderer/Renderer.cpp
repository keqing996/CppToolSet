#include "Renderer.h"
#include "Shader/ShaderProgram.h"
#include "Shader/VertexShader.h"
#include "Shader/PixelShader.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{
    Renderer* Renderer::Create(RendererApi api)
    {
        _api = api;

        switch (api)
        {
            case RendererApi::OpenGL:
            default:
                return new RhiOpenGL();
        }

        return nullptr;
    }

    RendererApi Renderer::GetApi()
    {
        return _api;
    }

    void Renderer::Render()
    {
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

        constexpr GLuint Indeics[] = { 0, 1, 2 };

        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        VertexShader* pVertexShader = VertexShader::Create(pvsCode);
        pVertexShader->Compile();

        PixelShader* pPixelShader = PixelShader::Create(pfsCode);
        pPixelShader->Compile();

        ShaderProgram* pShader = ShaderProgram::Create();
        pShader->AddVertexShader(pVertexShader);
        pShader->AddPixelShader(pPixelShader);
        pShader->Link();

        delete pVertexShader;
        delete pPixelShader;

        // Vertex Buffer Object
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vert), Vert, GL_STATIC_DRAW);

        // Vertex Array
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Vertex Attrib
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

        // Index Buffer
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // Index Buffer Data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indeics), Indeics, GL_STATIC_DRAW);


        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // ------------- draw
        pShader->Use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);


/*
        glBindVertexArray(0);



        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glFlush();
        */

        delete pShader;

        SwapBuffer();
    }
}