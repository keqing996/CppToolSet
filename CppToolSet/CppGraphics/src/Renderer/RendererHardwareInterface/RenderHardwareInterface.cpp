#include "RenderHardwareInterface.h"
#include "Renderer/RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{
    bool RenderHardwareInterface::IsSettleUp() const
    {
        return _setUp;
    }
}




