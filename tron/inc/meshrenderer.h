#ifndef OBJECT_H
#define OBJECT_H

#include "pch.h"
#include "transform.h"
#include "mesh.h"
#include "material.h"
#include "def.h"

namespace tron
{
    class MeshRenderer
    {
        ptr<Material> m_material;
        ptr<Mesh> m_mesh;
        ptr<Transform> m_transform;

        GLint m_projectionId = 0;
        GLint m_viewId = 0;
        GLint m_modelId = 0;

    public:
        MeshRenderer(ptr<Material> material, ptr<Mesh> mesh);

        void Bind(const glm::mat4& projection, const glm::mat4& view);

        virtual void Draw();

        ptr<Material> get_Material();
        void put_Material(ptr<Material> value);

        ptr<Mesh> get_Mesh();
        void put_Mesh(ptr<Mesh> mesh);

        ptr<Transform> get_Transform();

        __declspec(property(put=put_Material, get=get_Material))
        ptr<Material> Material;

        __declspec(property(put=put_Mesh, get=get_Mesh))
        ptr<Mesh> Mesh;

        __declspec(property(get=get_Transform))
        ptr<Transform> Transform;
    };
}

#endif //OBJECT_H
