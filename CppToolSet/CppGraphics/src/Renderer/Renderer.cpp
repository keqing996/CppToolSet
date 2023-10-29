#include <array>

#include "Renderer.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "Shader/VertexShader.h"
#include "Shader/PixelShader.h"

static constexpr const char* pvsCode = R"(
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

static constexpr const char* pfsCode = R"(
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

static constexpr std::array<float, 3 * (3 + 4)> Vert =  {
        // left
        -0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 1.7f, 1.0f,
        // right
        0.5f, -0.5f, 0.0f, 0.1f, 0.6f, 0.3f, 1.0f,
        // top
        0.0f, 0.5f, 0.0f, 0.8f, 0.2f, 0.1f, 1.0f
};

static constexpr std::array<unsigned int, 3> Indeices = { 0, 1, 2 };

namespace Renderer
{
    void Renderer::SetApi(RendererApi api)
    {
        _api = api;
    }

    RendererApi Renderer::GetApi()
    {
        return _api;
    }

    void Renderer::Render(RendererCommand* pCommand)
    {
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

        pCommand->Submit(pVertexArray, pShader);

        delete pVertexArray;
        delete pIndexBuffer;
        delete pVertexBuffer;
        delete pShader;
    }
}