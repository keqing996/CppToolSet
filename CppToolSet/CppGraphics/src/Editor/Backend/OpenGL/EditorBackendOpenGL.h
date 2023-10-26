#pragma once

#include "Editor/Backend/EditorBackend.h"

namespace Editor
{
    class EditorBackendOpenGL : public EditorBackend
    {
    public:
        virtual ~EditorBackendOpenGL();

    public:
        void SetUp() override;
        void Destroy() override;
        void BeforeUpdate() override;
        void AfterUpdate() override;
    };
}