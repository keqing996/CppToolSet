#pragma once

#include "Renderer/Buffer/VertexArray.h"

namespace Renderer
{
    class VertexArrayOpenGL : public VertexArray
    {
    public:
        VertexArrayOpenGL();
        ~VertexArrayOpenGL() override;

    public:
        void Bind() override;
        void UnBind() override;
        void AddVertexBuffer(const VertexBuffer* pVertexBuffer) override;
        void SetIndexBuffer(const IndexBuffer* pIndexBuffer) override;
        const std::vector<const VertexBuffer*>& GetVertexBufferVector() const override;
        const IndexBuffer* GetCurrentIndexBuffer() const override;

    private:
        unsigned int _vertexArrayId = 0;
        std::vector<const VertexBuffer*> _vertexBufferVec = {};
        const IndexBuffer* _pCurrentIndexBuffer = nullptr;
    };
}