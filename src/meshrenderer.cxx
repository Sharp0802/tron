#include "meshrenderer.h"

namespace tron
{
	MeshRenderer::MeshRenderer(ptr<tron::Material> material, ptr<tron::Mesh> mesh)
		: m_material(std::move(material)),
		  m_mesh(std::move(mesh)),
		  m_transform(std::make_shared<class Transform>())
	{
		Material->Bind();

		m_projectionId = Material->Program->GetLocation("projection");
		m_viewId       = Material->Program->GetLocation("view");
		m_modelId      = Material->Program->GetLocation("model");
	}

	void MeshRenderer::Bind(const glm::mat4& projection, const glm::mat4& view)
	{
		Material->Bind();
		Mesh->Bind();
		
		Shader::SetUnsafe(m_projectionId, projection);
		Shader::SetUnsafe(m_viewId, view);
		Shader::SetUnsafe(m_modelId, Transform->Matrix);
	}

	void MeshRenderer::Draw()
	{
		Mesh->Draw();
	}

	ptr<Material> MeshRenderer::get_Material()
	{
		return m_material;
	}

	void MeshRenderer::put_Material(ptr<tron::Material> value)
	{
		m_material = std::move(value);
	}

	ptr<Mesh> MeshRenderer::get_Mesh()
	{
		return m_mesh;
	}

	void MeshRenderer::put_Mesh(ptr<tron::Mesh> mesh)
	{
		m_mesh = std::move(mesh);
	}

	ptr<Transform> MeshRenderer::get_Transform()
	{
		return m_transform;
	}
}