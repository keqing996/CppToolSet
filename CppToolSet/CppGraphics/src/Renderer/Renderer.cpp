#include "Renderer.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{

    Renderer::Renderer(RendererApi api)
    {
        _api = api;

        switch (api)
        {
            case RendererApi::OpenGL:
            default:
                _pRhi = new RhiOpenGL();
        }
    }

    Renderer::~Renderer()
    {
        if (_pRhi != nullptr)
        {
            delete _pRhi;
            _pRhi = nullptr;
        }
    }

    RendererApi Renderer::GetApi() const
    {
        return _api;
    }

    void Renderer::Render()
    {
        _pRhi->Render();
    }

    void Renderer::Destroy()
    {
        _pRhi->Destroy();
    }
}