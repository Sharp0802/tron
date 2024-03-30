#ifndef OBJECT_H
#define OBJECT_H

#include "oop/oop.h"
#include "oop/specials/mesh.h"
#include "oop/specials/material.h"
#include "oop/component.h"

namespace tron::oop::components
{
    class MeshRenderer final : public Component
    {
        specials::Material* m_material = nullptr;
        specials::Mesh* m_mesh         = nullptr;

        GLint m_projectionId = -1;
        GLint m_viewId       = -1;
        GLint m_modelId      = -1;

    public:
        MeshRenderer(Actor* parent);

        void Bind();

        void Draw();

        mutable_(specials::Material*, Material);
        mutable_(specials::Mesh*, Mesh);
    };
}

#endif //OBJECT_H
