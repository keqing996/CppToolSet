#include "RhiImp.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

namespace Renderer
{
    bool RhiImp::IsSettleUp() const
    {
        return _setUp;
    }
}




