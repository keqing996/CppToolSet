#include "RenderHardwareInterface.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{
    bool RenderHardwareInterface::IsSettleUp() const
    {
        return _setUp;
    }
}




