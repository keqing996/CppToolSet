#include <array>

#include "Renderer.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "Shader/VertexShader.h"
#include "Shader/PixelShader.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"
#include "RendererHardwareInterface/OpenGL/Shader/ShaderDataTypeOpenGL.h"

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
        constexpr LPCSTR pvsCode = R"(
            #version 420 core

            layout (location = 0) in vec3 a_Position;
            layout (location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
)";

        constexpr LPCSTR pfsCode = R"(
            #version 420 core

            layout (location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
)";

        constexpr std::array<float, 3 * (3 + 4)> Vert =  {
                // left
                -0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 1.7f, 1.0f,
                // right
                0.5f, -0.5f, 0.0f, 0.1f, 0.6f, 0.3f, 1.0f,
                // top
                0.0f, 0.5f, 0.0f, 0.8f, 0.2f, 0.1f, 1.0f
        };

        constexpr std::array<unsigned int, 3> Indeices = { 0, 1, 2 };

        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ShaderProgram* pShader = ShaderProgram::Create();

        {
            VertexShader* pVertexShader = VertexShader::Create(pvsCode);
            pVertexShader->Compile();

            PixelShader* pPixelShader = PixelShader::Create(pfsCode);
            pPixelShader->Compile();

            pShader->AddVertexShader(pVertexShader);
            pShader->AddPixelShader(pPixelShader);

            delete pVertexShader;
            delete pPixelShader;
        }

        pShader->Link();

        VertexBuffer* pVertexBuffer = VertexBuffer::Create(Vert.data(), Vert.size());

        // Layout
        BufferLayout layout = {
                BufferElement {ShaderDataType::Float3, "a_Position"},
                BufferElement {ShaderDataType::Float4, "a_Color"},
                //BufferElement {ShaderDataType::Float3, "a_Normal"},
        };

        pVertexBuffer->SetLayout(std::move(layout));

        // Index Buffer
        IndexBuffer* pIndexBuffer = IndexBuffer::Create(Indeices.data(), Indeices.size());

        // Vertex Array
        VertexArray* pVertexArray = VertexArray::Create();
        pVertexArray->AddVertexBuffer(pVertexBuffer);
        pVertexArray->SetIndexBuffer(pIndexBuffer);

        // ------------- draw
        pVertexArray->Bind();
        pShader->Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        // ------------- draw

        delete pVertexArray;
        delete pIndexBuffer;
        delete pVertexBuffer;
        delete pShader;

        SwapBuffer();
    }
}