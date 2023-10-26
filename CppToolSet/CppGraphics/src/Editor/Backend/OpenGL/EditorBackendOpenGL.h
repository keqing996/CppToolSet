#pragma once

#include "Editor/Backend/EditorBackend.h"

namespace Editor
{
    class EditorBackendOpenGL : public EditorBackend
    {
    public:
        virtual ~EditorBackendOpenGL();

    public:
        void Destroy() override;

    private:
        void SetUpBackend() override;
    };
}