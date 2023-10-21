#pragma once

#include "Renderer/Buffer/VertexArray.h"

namespace Renderer
{
    class VertexArrayOpenGL : public VertexArray
    {
    public:
        VertexArrayOpenGL();
        ~VertexArrayOpenGL() override = default;

    public:
        void Bind() override;
        void UnBind() override;
        void AddVertexBuffer(const VertexBuffer* pVertexBuffer) override;
        void SetIndexBuffer(const IndexBuffer* pIndexBuffer) override;

    private:
        unsigned int _vertexArrayId = 0;
        std::vector<const VertexBuffer*> _vertexBufferVec = {};
        const IndexBuffer* _pCurrentIndexBuffer = nullptr;
    };
}