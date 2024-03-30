#include "oop/components/meshrenderer.h"
#include "oop/components/camera.h"
#include "oop/actor.h"
#include "oop/components/transform.h"

namespace tron::oop::components
{
    MeshRenderer::MeshRenderer(Actor* parent)
        : Component(GetType<MeshRenderer>(), parent, {
              .Updated = [](CObject* self, float)
              {
                  const auto renderer = dynamic_cast<MeshRenderer*>(self);
                  if (!renderer->m_material || !renderer->m_mesh)
                      return;

                  renderer->Bind();
                  renderer->Draw();
              }
          })
    {
    }

    void MeshRenderer::Bind()
    {
        auto& camera = Camera::Current();

        Material->Bind();
        Mesh->Bind();

        specials::Shader::SetUnsafe(m_projectionId, camera.Projection);
        specials::Shader::SetUnsafe(m_viewId, camera.View);
        specials::Shader::SetUnsafe(m_modelId,
            dynamic_cast<Transform*>(Parent->GetComponent(GetType<Transform>()))->Matrix);
    }

    void MeshRenderer::Draw()
    {
        Mesh->Draw();
    }

    getter_decl_(MeshRenderer, Material)
    {
        return m_material;
    }

    setter_decl_(MeshRenderer, Material)
    {
        m_material = value;

        Material->Bind();

        m_projectionId = Material->Program->GetLocation("projection");
        m_viewId       = Material->Program->GetLocation("view");
        m_modelId      = Material->Program->GetLocation("model");
    }

    getter_decl_(MeshRenderer, Mesh)
    {
        return m_mesh;
    }

    setter_decl_(MeshRenderer, Mesh)
    {
        m_mesh = value;
    }
}
